#include "graphics/app.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <vulkan/vulkan.h>

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
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

void debug(VulkanApp* app) {
    // BufferManager::get().printVertexBuffer(BufferManager::get().getAllocator().getVertexCount()+1);
    // BufferManager::get().printVertexBuffer(50);
    // BufferManager::get().printIndexBuffer(BufferManager::get().getAllocator().getIndexCount()+1);
    // BufferManager::get().printIndirectBuffer(BufferManager::get().getAllocator().getIndirectCount()+1);
}



void VulkanApp::initWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    window = glfwCreateWindow(1280, 720, "Minecraft-Vulkan", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

void VulkanApp::initVulkan() {
    Instance::get().createInstance();
    Instance::get().setupDebugMessenger();
    Instance::get().createSurface(window);

    Device::get().pickPhysicalDevice();
    Device::get().createLogicalDevice();

    Swapchain::get().createSwapChain(window);
    Swapchain::get().createImageViews();

    GraphicPipeline::get().createRenderPass();
    GraphicPipeline::get().createDescriptorSetLayout();
    GraphicPipeline::get().createGraphicsPipeline();
    ComputePipeline::get().createDescriptorSetLayout();
    ComputePipeline::get().createComputePipeline();

    Renderer::get().createCommandPool();
    Device::get().createDepthResources();
    Renderer::get().createFramebuffers();

    Texture::get().createTextureImage();
    Texture::get().createTextureImageView();
    Texture::get().createTextureSampler();

    BufferManager::get().createBuffers();
    BufferManager::get().createUniformBuffers();

    Descriptor::get().createDescriptorPool();
    Descriptor::get().createDescriptorSets();

    Renderer::get().createCommandBuffers();
    Renderer::get().createComputeCommandBuffers();
    Renderer::get().createSyncObjects();
} 

void VulkanApp::render() {
    updateDeltaTime();
    glfwPollEvents();
    camera.update(deltaTime);
    drawFrame();
}

void VulkanApp::init() {
    initWindow();
    initVulkan();

    camera.updateProjection(Swapchain::get().getAspectRatio());
}

void VulkanApp::cleanup() {
    debug(this);

    vkDeviceWaitIdle(Device::get().getDevice());

    Swapchain::get().cleanup();
    GraphicPipeline::get().cleanup();
    ComputePipeline::get().cleanup();
    BufferManager::get().cleanupUniformBuffer();
    Descriptor::get().cleanup();
    Texture::get().cleanup();
    GraphicPipeline::get().cleanupDescriptorSetLayout();
    ComputePipeline::get().cleanupDescriptorSetLayout();
    BufferManager::get().cleanupBuffers();
    Renderer::get().cleanup();
    Device::get().cleanup();
    Instance::get().cleanup();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void VulkanApp::drawFrame() {
    std::vector<VkSemaphore> waitSemaphores = {Renderer::get().getCurrentImageAvailableSemaphores()};
    std::vector<VkPipelineStageFlags> waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    // if (!generated) computeShader(waitSemaphores, waitStages);

    vkWaitForFences(Device::get().getDevice(), 1, &Renderer::get().getCurrentInFlightFences(), VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(Device::get().getDevice(), Swapchain::get().getSwapChain(), UINT64_MAX, Renderer::get().getCurrentImageAvailableSemaphores(), VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        Swapchain::get().recreateSwapChain(window);
        camera.updateProjection(Swapchain::get().getAspectRatio());
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    BufferManager::get().updateUniformBuffer(camera.getProjectionMatrix()*camera.getViewMatrix());

    vkResetFences(Device::get().getDevice(), 1, &Renderer::get().getCurrentInFlightFences());

    vkResetCommandBuffer(Renderer::get().getCurrentCommandBuffers(), /*VkCommandBufferResetFlagBits*/ 0);
    recordCommandBuffer(imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = waitSemaphores.size();
    submitInfo.pWaitSemaphores = waitSemaphores.data();
    submitInfo.pWaitDstStageMask = waitStages.data();

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &Renderer::get().getCurrentCommandBuffers();

    VkSemaphore signalSemaphores[] = {Renderer::get().getCurrentRenderFinishedSemaphores()};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(Device::get().getGraphicsQueue(), 1, &submitInfo, Renderer::get().getCurrentInFlightFences()) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {Swapchain::get().getSwapChain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(Device::get().getPresentQueue(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        framebufferResized = false;
        Swapchain::get().recreateSwapChain(window);
        camera.updateProjection(Swapchain::get().getAspectRatio());
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    Renderer::get().incrementeCurrentFrame();
}

void VulkanApp::recordCommandBuffer(uint32_t imageIndex) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(Renderer::get().getCurrentCommandBuffers(), &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = GraphicPipeline::get().getRenderPass();
    renderPassInfo.framebuffer = Swapchain::get().getSwapChainFramebuffers()[imageIndex];
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = Swapchain::get().getSwapChainExtent();

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 1.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(Renderer::get().getCurrentCommandBuffers(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(Renderer::get().getCurrentCommandBuffers(), VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicPipeline::get().getGraphicsPipeline());

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) Swapchain::get().getSwapChainExtent().width;
        viewport.height = (float) Swapchain::get().getSwapChainExtent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(Renderer::get().getCurrentCommandBuffers(), 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = Swapchain::get().getSwapChainExtent();
        vkCmdSetScissor(Renderer::get().getCurrentCommandBuffers(), 0, 1, &scissor);

        VkBuffer vertexBuffers[] = {BufferManager::get().getVertexBuffers().getBuffer()};
        VkDeviceSize offsets[] = {0};

        vkCmdBindVertexBuffers(Renderer::get().getCurrentCommandBuffers(), 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(Renderer::get().getCurrentCommandBuffers(), BufferManager::get().getIndexBuffers().getBuffer(), 0, VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(Renderer::get().getCurrentCommandBuffers(), VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicPipeline::get().getPipelineLayout(), 0, 1, &(Descriptor::get().getDescriptorSets())[Renderer::get().getCurrentFrame()], 0, nullptr);

        // vkCmdDrawIndexed(Renderer::get().getCurrentCommandBuffers(), BufferManager::get().getAllocator().getNumberIndex(), 1, 0, 0, 0); // Va faloir changer ça (recup le nb de face a afficher), le 2eme arg c'est le nb d'index

        vkCmdDrawIndexedIndirect(
            Renderer::get().getCurrentCommandBuffers(),
            BufferManager::get().getAllocator().getIndirectBuffer().getBuffer(),  // Buffer contenant les commandes
            0,                                                                    // Offset du premier DrawIndirectCommand
            BufferManager::get().getAllocator().getIndirectCount(),               // Nombre de draw calls (10 dans ton cas)
            sizeof(DrawIndirectCommand)                                           // Taille d’une commande
        );

    vkCmdEndRenderPass(Renderer::get().getCurrentCommandBuffers());

    if (vkEndCommandBuffer(Renderer::get().getCurrentCommandBuffers()) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
}

void VulkanApp::recordComputeCommandBuffer() {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(Renderer::get().getCurrentComputeCommandBuffers(), &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording compute command buffer!");
    }

    vkCmdBindPipeline(Renderer::get().getCurrentComputeCommandBuffers(), VK_PIPELINE_BIND_POINT_COMPUTE, ComputePipeline::get().getComputePipeline());

    vkCmdBindDescriptorSets(Renderer::get().getCurrentComputeCommandBuffers(), VK_PIPELINE_BIND_POINT_COMPUTE, ComputePipeline::get().getComputePipelineLayout(), 0, 1, &Descriptor::get().getComputeDescriptorSets(), 0, nullptr);

    uint32_t bufferSize = blockUpdate.size();
    vkCmdPushConstants(Renderer::get().getCurrentComputeCommandBuffers(), ComputePipeline::get().getComputePipelineLayout(), VK_SHADER_STAGE_COMPUTE_BIT, 0, sizeof(uint32_t), &bufferSize);

    BufferManager::get().updateUpdateVoxelBuffer(blockUpdate);

    vkCmdDispatch(Renderer::get().getCurrentComputeCommandBuffers(), blockUpdate.size(), 1, 1);

    if (vkEndCommandBuffer(Renderer::get().getCurrentComputeCommandBuffers()) != VK_SUCCESS) {
        throw std::runtime_error("failed to record compute command buffer!");
    }
}

void VulkanApp::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<VulkanApp*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

void VulkanApp::computeShader(std::vector<VkSemaphore>& waitSemaphores, std::vector<VkPipelineStageFlags>& waitStages) {
    waitSemaphores.push_back(Renderer::get().getCurrentComputeFinishedSemaphores());
    waitStages.push_back(VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT);

    generated = 1;
     
    vkWaitForFences(Device::get().getDevice(), 1, &Renderer::get().getCurrentComputeInFlightFences(), VK_TRUE, UINT64_MAX);
    vkResetFences(Device::get().getDevice(), 1, &Renderer::get().getCurrentComputeInFlightFences());

    vkResetCommandBuffer(Renderer::get().getCurrentComputeCommandBuffers(), 0);
    recordComputeCommandBuffer();

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &Renderer::get().getCurrentComputeCommandBuffers();
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &Renderer::get().getCurrentComputeFinishedSemaphores();

    if (vkQueueSubmit(Device::get().getComputeQueue(), 1, &submitInfo, Renderer::get().getCurrentComputeInFlightFences()) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit compute command buffer!");
    };

    std::cout << "Compute shader ok !" << std::endl;
}

void VulkanApp::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    VulkanApp* app = static_cast<VulkanApp*>(glfwGetWindowUserPointer(window));

    if (!app) return;

    int code = app->camera.processKeyboard(key, action);

    if (!code) return;

    switch (code)
    {
    case 1:
        glfwSetWindowShouldClose(window, true);
        break;

    default:
        break;
    }

}

void VulkanApp::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    VulkanApp* app = static_cast<VulkanApp*>(glfwGetWindowUserPointer(window));
    if (app) {
        double width = Swapchain::get().getSwapChainExtent().width;
        double height = Swapchain::get().getSwapChainExtent().height;

        app->camera.processMouse(width/2-xpos, height/2-ypos);

        glfwSetCursorPos(window, width/2, height/2);
    }
}

void VulkanApp::updateDeltaTime() {
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
}