#include "core/vulkan_renderer.h" 

#include "core/vulkan_config.h"
#include "core/vulkan_device.h"
#include "core/vulkan_pipeline.h"
#include "core/vulkan_swapchain.h"

void VulkanRenderer::createCommandPool() {
    QueueFamilyIndices queueFamilyIndices = vulkanDevice.findQueueFamilies(vulkanDevice.getPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsAndComputeFamily.value();

    if (vkCreateCommandPool(vulkanDevice.getDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics command pool!");
    }
}

void VulkanRenderer::createCommandBuffers() {
    commandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if (vkAllocateCommandBuffers(vulkanDevice.getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void VulkanRenderer::createComputeCommandBuffers() {
    computeCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)computeCommandBuffers.size();

    if (vkAllocateCommandBuffers(vulkanDevice.getDevice(), &allocInfo, computeCommandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate compute command buffers!");
    }
}

void VulkanRenderer::createSyncObjects() {
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    computeInFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    computeFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(vulkanDevice.getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(vulkanDevice.getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(vulkanDevice.getDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
        if (vkCreateSemaphore(vulkanDevice.getDevice(), &semaphoreInfo, nullptr, &computeFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(vulkanDevice.getDevice(), &fenceInfo, nullptr, &computeInFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create compute synchronization objects for a frame!");
        }
    }
}

void VulkanRenderer::createFramebuffers() {
    for (size_t i = 0; i < vulkanSwapchain.getSwapChainImageViews().size(); i++) {
        std::array<VkImageView, 2> attachments = {
            vulkanSwapchain.getSwapChainImageViews()[i],
            vulkanDevice.getDepthImageView()
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = vulkanPipeline.getRenderPass();
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = vulkanSwapchain.getSwapChainExtent().width;
        framebufferInfo.height = vulkanSwapchain.getSwapChainExtent().height;
        framebufferInfo.layers = 1;

        VkFramebuffer frameBuffer;
        if (vkCreateFramebuffer(vulkanDevice.getDevice(), &framebufferInfo, nullptr, &frameBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }

        vulkanSwapchain.addSwapChainFramebuffers(frameBuffer);
    }
}

void VulkanRenderer::cleanup() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(vulkanDevice.getDevice(), renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(vulkanDevice.getDevice(), imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(vulkanDevice.getDevice(), computeFinishedSemaphores[i], nullptr);
        vkDestroyFence(vulkanDevice.getDevice(), inFlightFences[i], nullptr);
        vkDestroyFence(vulkanDevice.getDevice(), computeInFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(vulkanDevice.getDevice(), commandPool, nullptr);
}

void VulkanRenderer::incrementeCurrentFrame() {
    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}