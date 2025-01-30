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
    void createComputeCommandPool(uint32_t computeQueueFamily);
    void createComputeCommandBuffers();
    void createSyncObjects();
    void createFramebuffers();
    void cleanup();

    VulkanRenderer(VulkanDevice& pvulkanDevice, VulkanSwapchain& pvulkanSwapchain, VulkanPipeline& pvulkanPipeline)
    : vulkanDevice(pvulkanDevice), vulkanSwapchain(pvulkanSwapchain), vulkanPipeline(pvulkanPipeline), currentFrame(0) {};

    VkCommandPool getCommandPool() const { return commandPool; }
    VkCommandBuffer* getCurrentCommandBuffers() { return &commandBuffers[currentFrame]; }
    VkCommandBuffer* getCommandBuffer(int index) { return &commandBuffers[index]; }
    VkCommandBuffer* getCurrentComputeCommandBuffers() { return &computeCommandBuffers[currentFrame]; }
    std::vector<VkFence>* getInFlightFences() { return &inFlightFences; }
    std::vector<VkFence>* getComputeInFlightFences() { return &computeInFlightFences; }
    uint32_t getCurrentFrame() const { return currentFrame; }
    const VkFence* getCurrentInFlightFences() const { return &inFlightFences[currentFrame]; }
    const VkFence* getCurrentComputeInFlightFences() const { return &computeInFlightFences[currentFrame]; }
    const VkSemaphore* getCurrentImageAvailableSemaphores() const { return &imageAvailableSemaphores[currentFrame]; }
    const VkSemaphore* getCurrentRenderFinishedSemaphores() const { return &renderFinishedSemaphores[currentFrame]; }
    const VkSemaphore* getCurrentComputeFinishedSemaphores() const { return &computeFinishedSemaphores[currentFrame]; }

    void incrementeCurrentFrame();

private:
    VkCommandPool commandPool;
    
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    std::vector<VkCommandBuffer> computeCommandBuffers;
    std::vector<VkSemaphore> computeFinishedSemaphores;
    std::vector<VkFence> computeInFlightFences;

    uint32_t currentFrame;

    VulkanDevice& vulkanDevice;
    VulkanSwapchain& vulkanSwapchain;
    VulkanPipeline& vulkanPipeline;
    
};

#endif // VULKAN_RENDERER_HPP