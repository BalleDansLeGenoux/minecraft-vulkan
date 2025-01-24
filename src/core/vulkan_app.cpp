#include "core/vulkan_app.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdint>
#include <limits>
#include <array>
#include <optional>
#include <set>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

void VulkanApp::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(1280, 720, "Minecraft-Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

void VulkanApp::initVulkan() {
    vulkanInstance.createInstance();
    vulkanInstance.setupDebugMessenger();
    vulkanInstance.createSurface();
    vulkanDevice.pickPhysicalDevice();
    vulkanDevice.createLogicalDevice();
    vulkanSwapchain.createSwapChain();
    vulkanSwapchain.createImageViews();
    vulkanPipeline.createRenderPass();
    vulkanPipeline.createDescriptorSetLayout();
    vulkanPipeline.createGraphicsPipeline();
    vulkanRenderer.createCommandPool();
    vulkanDevice.createDepthResources();
    vulkanRenderer.createFramebuffers();
    vulkanTexture.createTextureImage();
    vulkanTexture.createTextureImageView();
    vulkanTexture.createTextureSampler();
    vulkanBuffer.createVertexBuffer();
    vulkanBuffer.createIndexBuffer();
    vulkanBuffer.createUniformBuffers();
    vulkanDescriptor.createDescriptorPool();
    vulkanDescriptor.createDescriptorSets();
    vulkanRenderer.createCommandBuffers();
    vulkanRenderer.createSyncObjects();
} 

void VulkanApp::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(vulkanDevice.getDevice());
}

void VulkanApp::run() {
    initWindow();
    initVulkan();
    mainLoop();
    cleanup();
}

void VulkanApp::drawFrame() {
    vkWaitForFences(vulkanDevice.getDevice(), 1, vulkanRenderer.getCurrentInFlightFences(), VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(vulkanDevice.getDevice(), vulkanSwapchain.getSwapChain(), UINT64_MAX, *vulkanRenderer.getCurrentImageAvailableSemaphores(), VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        vulkanSwapchain.recreateSwapChain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    vulkanBuffer.updateUniformBuffer();

    vkResetFences(vulkanDevice.getDevice(), 1, vulkanRenderer.getCurrentInFlightFences());

    vkResetCommandBuffer(*vulkanRenderer.getCurrentCommandBuffers(), /*VkCommandBufferResetFlagBits*/ 0);
    recordCommandBuffer(imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {*vulkanRenderer.getCurrentImageAvailableSemaphores()};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = vulkanRenderer.getCurrentCommandBuffers();

    VkSemaphore signalSemaphores[] = {*vulkanRenderer.getCurrentRenderFinishedSemaphores()};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(vulkanDevice.getGraphicsQueue(), 1, &submitInfo, *vulkanRenderer.getCurrentInFlightFences()) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {vulkanSwapchain.getSwapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(vulkanDevice.getPresentQueue(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        vulkanSwapchain.recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    vulkanRenderer.incrementeCurrentFrame();
}

void VulkanApp::recordCommandBuffer(uint32_t imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(*vulkanRenderer.getCurrentCommandBuffers(), &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = vulkanPipeline.getRenderPass();
    renderPassInfo.framebuffer = vulkanSwapchain.getSwapChainFramebuffers()[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = vulkanSwapchain.getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(*vulkanRenderer.getCurrentCommandBuffers(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(*vulkanRenderer.getCurrentCommandBuffers(), VK_PIPELINE_BIND_POINT_GRAPHICS, *vulkanPipeline.getGraphicsPipeline());

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) vulkanSwapchain.getSwapChainExtent().width;
        viewport.height = (float) vulkanSwapchain.getSwapChainExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(*vulkanRenderer.getCurrentCommandBuffers(), 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = vulkanSwapchain.getSwapChainExtent();
        vkCmdSetScissor(*vulkanRenderer.getCurrentCommandBuffers(), 0, 1, &scissor);

        VkBuffer vertexBuffers[] = {*vulkanBuffer.getVertexBuffer()};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(*vulkanRenderer.getCurrentCommandBuffers(), 0, 1, vertexBuffers, offsets);

        vkCmdBindIndexBuffer(*vulkanRenderer.getCurrentCommandBuffers(), *vulkanBuffer.getIndexBuffer(), 0, VK_INDEX_TYPE_UINT16);

        vkCmdBindDescriptorSets(*vulkanRenderer.getCurrentCommandBuffers(), VK_PIPELINE_BIND_POINT_GRAPHICS, *vulkanPipeline.getPipelineLayout(), 0, 1, &(*vulkanDescriptor.getDescriptorSets())[vulkanRenderer.getCurrentFrame()], 0, nullptr);

        vkCmdDrawIndexed(*vulkanRenderer.getCurrentCommandBuffers(), static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

    vkCmdEndRenderPass(*vulkanRenderer.getCurrentCommandBuffers());

    if (vkEndCommandBuffer(*vulkanRenderer.getCurrentCommandBuffers()) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void VulkanApp::cleanup() {
    vulkanSwapchain.cleanup();
    vulkanPipeline.cleanup();
    vulkanBuffer.cleanupUniform();
    vulkanDescriptor.cleanup();
    vulkanTexture.cleanup();
    vulkanPipeline.cleanupDescriptorSetLayout();
    vulkanBuffer.cleanupVertexIndices();
    vulkanRenderer.cleanup();
    vulkanDevice.cleanup();
    vulkanInstance.cleanup();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void VulkanApp::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<VulkanApp*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}