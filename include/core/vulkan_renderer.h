#ifndef VULKAN_RENDERER_HPP
#define VULKAN_RENDERER_HPP

#include <vulkan/vulkan.h>
#include <vector>

class VulkanDevice;
class VulkanSwapchain;
class VulkanPipeline;

class VulkanRenderer {
public:
    void createCommandPool();
    void createCommandBuffers();
    void createSyncObjects();
    void createFramebuffers();
    void cleanup();

    VulkanRenderer(VulkanDevice& pvulkanDevice, VulkanSwapchain& pvulkanSwapchain, VulkanPipeline& pvulkanPipeline)
    : vulkanDevice(pvulkanDevice), vulkanSwapchain(pvulkanSwapchain), vulkanPipeline(pvulkanPipeline), currentFrame(0) {};

    VkCommandPool getCommandPool() const { return commandPool; }
    const VkCommandBuffer* getCurrentCommandBuffers() const { return &commandBuffers[currentFrame]; }
    std::vector<VkFence> getInFlightFences() const { return inFlightFences; }
    uint32_t getCurrentFrame() const { return currentFrame; }
    const VkFence* getCurrentInFlightFences() const { return &inFlightFences[currentFrame]; }
    const VkSemaphore* getCurrentImageAvailableSemaphores() const { return &imageAvailableSemaphores[currentFrame]; }
    const VkSemaphore* getCurrentRenderFinishedSemaphores() const { return &renderFinishedSemaphores[currentFrame]; }

    void incrementeCurrentFrame();

private:
    VkCommandPool commandPool;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;
    uint32_t currentFrame;

    VulkanDevice& vulkanDevice;
    VulkanSwapchain& vulkanSwapchain;
    VulkanPipeline& vulkanPipeline;
    
};

#endif // VULKAN_RENDERER_HPP