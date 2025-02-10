#include "core/vulkan/app.h"

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

// void printVertexBuffer(Buffer vulkanBuffer, Device device, size_t sizeToPrint) {
//     // ceci est un test, oui monsieur, un test

//     VkBuffer stagingBuffer;
//     VkDeviceMemory stagingBufferMemory;
//     VkDeviceSize bufferSize = sizeof(Vertex) * sizeToPrint;
//     // VkDeviceSize bufferSize = sizeof(Vertex) * vertices.size();
    
//     vulkanBuffer.createBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

//     if (!stagingBuffer || !stagingBufferMemory) {
//         throw std::runtime_error("Failed to create staging buffer!");
//     }

//     // Copier le contenu du tampon de sortie vers le tampon de staging
//     vulkanBuffer.copyBuffer(*vulkanBuffer.getVertexBuffer(), stagingBuffer, bufferSize);

//     // Mapper la mémoire pour accéder aux données
//     void* data;
//     vkMapMemory(device.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
//     Vertex* resultData = static_cast<Vertex*>(data);

//     std::cout << "Buffer capacity : " << BASE_BUFFER_CAPACITY << std::endl;

//     // Vérifier les résultats
//     for (int i = 0; i < sizeToPrint; ++i) {
//         std::cout << "  pos [" << i << "] = " << resultData[i].pos.x << " | " << resultData[i].pos.y << " | " << resultData[i].pos.z << std::endl;
//         // std::cout << "     color [" << i << "] = " << resultData[i].color.x << " | " << resultData[i].color.y << " | " << resultData[i].color.z << std::endl;
//         // std::cout << "  texCoord [" << i << "] = " << resultData[i].texCoord.x << " | " << resultData[i].texCoord.y << std::endl << std::endl;
//         // std::cout << "Result[" << i << "] = " << resultData[i] << std::endl;
//     }

//     vkUnmapMemory(device.getDevice(), stagingBufferMemory);
    
//     vkDestroyBuffer(device.getDevice(), stagingBuffer, nullptr);
//     vkFreeMemory(device.getDevice(), stagingBufferMemory, nullptr);
// }

// void printIndexBuffer(VulkanBuffer vulkanBuffer, VulkanDevice device, size_t sizeToPrint) {
//     // ceci est un test, oui monsieur, un test

//     VkBuffer stagingBuffer;
//     VkDeviceMemory stagingBufferMemory;
//     VkDeviceSize bufferSize = sizeof(uint32_t) * sizeToPrint;
//     // VkDeviceSize bufferSize = sizeof(uint32_t) * indices.size();
    
//     vulkanBuffer.createBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

//     if (!stagingBuffer || !stagingBufferMemory) {
//         throw std::runtime_error("Failed to create staging buffer!");
//     }

//     // Copier le contenu du tampon de sortie vers le tampon de staging
//     vulkanBuffer.copyBuffer(*vulkanBuffer.getIndexBuffer(), stagingBuffer, bufferSize);

//     // Mapper la mémoire pour accéder aux données
//     void* data;
//     vkMapMemory(device.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
//     uint32_t* resultData = static_cast<uint32_t*>(data);

//     std::cout << "Buffer capacity : " << BASE_BUFFER_CAPACITY << std::endl;

//     // Vérifier les résultats
//     for (int i = 0; i < sizeToPrint; ++i) {
//         std::cout << "  index [" << i << "] = " << resultData[i] << std::endl;
//     }

//     vkUnmapMemory(device.getDevice(), stagingBufferMemory);
    
//     vkDestroyBuffer(device.getDevice(), stagingBuffer, nullptr);
//     vkFreeMemory(device.getDevice(), stagingBufferMemory, nullptr);
// }

// void printIndexBufferLikeUpdateBuffer(VulkanBuffer vulkanBuffer, VulkanDevice device, size_t size) {
//     // ceci est un test, oui monsieur, un test

//     VkBuffer stagingBuffer;
//     VkDeviceMemory stagingBufferMemory;
//     VkDeviceSize bufferSize = sizeof(BlockUpdate) * size;
//     // VkDeviceSize bufferSize = sizeof(uint32_t) * indices.size();
    
//     vulkanBuffer.createBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

//     if (!stagingBuffer || !stagingBufferMemory) {
//         throw std::runtime_error("Failed to create staging buffer!");
//     }

//     // Copier le contenu du tampon de sortie vers le tampon de staging
//     vulkanBuffer.copyBuffer(*vulkanBuffer.getUpdateVoxelBuffer(), stagingBuffer, bufferSize);

//     // Mapper la mémoire pour accéder aux données
//     void* data;
//     vkMapMemory(device.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
//     BlockUpdate* resultData = static_cast<BlockUpdate*>(data);

//     std::cout << "Buffer capacity : " << BASE_BUFFER_CAPACITY << std::endl;

//     // Vérifier les résultats
//     for (int i = 0; i < size; ++i) {
//         std::cout << "  index [" << i << "].chunkPos.x = " << resultData[i].chunkPos.x << std::endl;
//         std::cout << "  index [" << i << "].chunkPos.y = " << resultData[i].chunkPos.y << std::endl;
//         std::cout << "  index [" << i << "].blockPos.x = " << resultData[i].blockPos.x << std::endl;
//         std::cout << "  index [" << i << "].blockPos.y = " << resultData[i].blockPos.y << std::endl;
//         std::cout << "  index [" << i << "].blockPos.z = " << resultData[i].blockPos.z << std::endl;
//         std::cout << "  index [" << i << "].blockID = " << resultData[i].blockID.x << std::endl;
//     }

//     std::cout << std::endl;

//     vkUnmapMemory(device.getDevice(), stagingBufferMemory);
    
//     vkDestroyBuffer(device.getDevice(), stagingBuffer, nullptr);
//     vkFreeMemory(device.getDevice(), stagingBufferMemory, nullptr);
// }

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

    // vulkanBuffer.createVertexBuffer();
    // vulkanBuffer.createIndexBuffer();
    // vulkanBuffer.createVoxelBuffer();
    // vulkanBuffer.createUpdateVoxelBuffer();
    // vulkanBuffer.createUniformBuffers();

    Descriptor::get().createDescriptorPool();
    Descriptor::get().createDescriptorSets();
    Renderer::get().createCommandBuffers();
    Renderer::get().createComputeCommandBuffers();
    Renderer::get().createSyncObjects();
} 

void VulkanApp::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        updateDeltaTime();
        glfwPollEvents();
        camera.update(deltaTime);
        drawFrame();
    }

    vkDeviceWaitIdle(Device::get().getDevice());
}

void VulkanApp::run() {
    initWindow();
    initVulkan();

    camera.updateProjection(Swapchain::get().getAspectRatio());
    
    mainLoop();

    // printVertexBuffer(vulkanBuffer, vulkanDevice, 4*200*200);
    // printIndexBuffer(vulkanBuffer, vulkanDevice, 12);
    // printIndexBufferLikeUpdateBuffer(vulkanBuffer, vulkanDevice, 1);

    vkDeviceWaitIdle(Device::get().getDevice());
    
    cleanup();
}

void VulkanApp::cleanup() {
    Swapchain::get().cleanup();
    GraphicPipeline::get().cleanup();
    ComputePipeline::get().cleanup();
    // vulkanBuffer.cleanupUniform();
    Descriptor::get().cleanup();
    Texture::get().cleanup();
    GraphicPipeline::get().cleanupDescriptorSetLayout();
    ComputePipeline::get().cleanupDescriptorSetLayout();
    // vulkanBuffer.cleanupVertexIndices();
    Renderer::get().cleanup();
    Device::get().cleanup();
    Instance::get().cleanup();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void VulkanApp::drawFrame() {
    std::vector<VkSemaphore> waitSemaphores = {Renderer::get().getCurrentImageAvailableSemaphores()};
    std::vector<VkPipelineStageFlags> waitStages = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};

    if (!generated) computeShader(waitSemaphores, waitStages);

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

    // vulkanBuffer.updateUniformBuffer(camera.getProjectionMatrix()*camera.getViewMatrix());

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
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
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

        // VkBuffer vertexBuffers[] = {*vulkanBuffer.getVertexBuffer()};
        VkDeviceSize offsets[] = {0};

        // vkCmdBindVertexBuffers(Renderer::get().getCurrentCommandBuffers(), 0, 1, vertexBuffers, offsets);
        // vkCmdBindIndexBuffer(Renderer::get().getCurrentCommandBuffers(), *vulkanBuffer.getIndexBuffer(), 0, VK_INDEX_TYPE_UINT32);
        vkCmdBindDescriptorSets(Renderer::get().getCurrentCommandBuffers(), VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicPipeline::get().getPipelineLayout(), 0, 1, &(Descriptor::get().getDescriptorSets())[Renderer::get().getCurrentFrame()], 0, nullptr);

        vkCmdDrawIndexed(Renderer::get().getCurrentCommandBuffers(), 6*blockUpdate.size(), 1, 0, 0, 0); // Va faloir changer ça (recup le nb de face a afficher), le 2eme arg c'est le nb d'index

    vkCmdEndRenderPass(Renderer::get().getCurrentCommandBuffers());

    if (vkEndCommandBuffer(Renderer::get().getCurrentCommandBuffers()) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }

    // blockUpdate.clear();
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

    // vulkanBuffer.updateVoxelsBuffer(blockUpdate);

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

    case 2:
        std::cout << "[FPS] " << 1.0f/app->getDeltaTime() << std::endl;
    
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