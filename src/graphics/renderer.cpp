#include "graphics/renderer.h"

#include <array>
#include <stdexcept>

#include "core/config.h"
#include "graphics/device.h"
#include "graphics/graphic_pipeline.h"

void Renderer::createCommandPool() {
    QueueFamilyIndices queueFamilyIndices = Device::get().findQueueFamilies(Device::get().getPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsAndComputeFamily.value();

    if (vkCreateCommandPool(Device::get().getDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics command pool!");
    }
}

void Renderer::createCommandBuffers() {
    commandBuffers.resize(FRAME_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if (vkAllocateCommandBuffers(Device::get().getDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    commandBufferState.resize(FRAME_IN_FLIGHT);
    for (int i = 0; i < FRAME_IN_FLIGHT; i++) {
        commandBufferState.at(i) = false;
    }

    VkCommandBufferAllocateInfo copyAllocInfo{};
    copyAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    copyAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    copyAllocInfo.commandPool = commandPool;
    copyAllocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(Device::get().getDevice(), &copyAllocInfo, &copyCommandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void Renderer::createComputeCommandBuffers() {
    computeCommandBuffers.resize(FRAME_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)computeCommandBuffers.size();

    if (vkAllocateCommandBuffers(Device::get().getDevice(), &allocInfo, computeCommandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate compute command buffers!");
    }
}

void Renderer::createSyncObjects() {
    imageAvailableSemaphores.resize(FRAME_IN_FLIGHT);
    renderFinishedSemaphores.resize(FRAME_IN_FLIGHT);
    computeInFlightFences.resize(FRAME_IN_FLIGHT);
    computeFinishedSemaphores.resize(FRAME_IN_FLIGHT);
    inFlightFences.resize(FRAME_IN_FLIGHT);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < FRAME_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(Device::get().getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(Device::get().getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(Device::get().getDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
        if (vkCreateSemaphore(Device::get().getDevice(), &semaphoreInfo, nullptr, &computeFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(Device::get().getDevice(), &fenceInfo, nullptr, &computeInFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create compute synchronization objects for a frame!");
        }
    }
}

void Renderer::createFramebuffers() {
    for (size_t i = 0; i < Swapchain::get().getSwapChainImageViews().size(); i++) {
        std::array<VkImageView, 2> attachments = {
            Swapchain::get().getSwapChainImageViews()[i],
            Device::get().getDepthImageView()
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = GraphicPipeline::get().getRenderPass();
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = Swapchain::get().getSwapChainExtent().width;
        framebufferInfo.height = Swapchain::get().getSwapChainExtent().height;
        framebufferInfo.layers = 1;

        VkFramebuffer frameBuffer;
        if (vkCreateFramebuffer(Device::get().getDevice(), &framebufferInfo, nullptr, &frameBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }

        Swapchain::get().addSwapChainFramebuffers(frameBuffer);
    }
}

void Renderer::resetCommandBuffers() {
    for (int i = 0; i < FRAME_IN_FLIGHT; i++) {
        vkResetCommandBuffer(commandBuffers[i], 0);
        commandBufferState[i] = false;
    }
}

void Renderer::resetCopyCommandBuffer() {
    vkResetCommandBuffer(copyCommandBuffer, 0);
}

void Renderer::cleanup() {
    for (size_t i = 0; i < FRAME_IN_FLIGHT; i++) {
        vkDestroySemaphore(Device::get().getDevice(), renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(Device::get().getDevice(), imageAvailableSemaphores[i], nullptr);
        vkDestroySemaphore(Device::get().getDevice(), computeFinishedSemaphores[i], nullptr);
        vkDestroyFence(Device::get().getDevice(), inFlightFences[i], nullptr);
        vkDestroyFence(Device::get().getDevice(), computeInFlightFences[i], nullptr);
    }

    vkDestroyCommandPool(Device::get().getDevice(), commandPool, nullptr);
}

void Renderer::incrementeCurrentFrame() {
    currentFrame = (currentFrame + 1) % FRAME_IN_FLIGHT;
}