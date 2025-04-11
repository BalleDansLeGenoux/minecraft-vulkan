#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include <vulkan/vulkan.h>
#include <vector>

class Renderer {
public:
    static Renderer& get() {
        static Renderer instance;
        return instance;
    }

    void createCommandPool();
    void createCommandBuffers();
    void createComputeCommandPool(uint32_t computeQueueFamily);
    void createComputeCommandBuffers();
    void createSyncObjects();
    void createFramebuffers();
    void resetCommandBuffers();
    void cleanup();

    const VkCommandPool& getCommandPool() { return commandPool; }

    const VkCommandBuffer& getCurrentCommandBuffers() { return commandBuffers[currentFrame]; }
    const VkCommandBuffer& getCommandBuffer(int index) { return commandBuffers[index]; }

    const bool getCurrentCommandBuffersState() const { return commandBufferState[currentFrame]; }
    const void setCurrentCommandBuffersState(bool p_state) { commandBufferState[currentFrame] = p_state; }

    const VkCommandBuffer& getCurrentComputeCommandBuffers() { return computeCommandBuffers[currentFrame]; }
    const std::vector<VkFence>& getInFlightFences() { return inFlightFences; }
    const std::vector<VkFence>& getComputeInFlightFences() { return computeInFlightFences; }

    const uint32_t& getCurrentFrame() { return currentFrame; }
    const VkFence& getCurrentInFlightFences() { return inFlightFences[currentFrame]; }
    const VkFence& getCurrentComputeInFlightFences() { return computeInFlightFences[currentFrame]; }

    const VkSemaphore& getCurrentImageAvailableSemaphores() { return imageAvailableSemaphores[currentFrame]; }
    const VkSemaphore& getCurrentRenderFinishedSemaphores() { return renderFinishedSemaphores[currentFrame]; }
    const VkSemaphore& getCurrentComputeFinishedSemaphores() { return computeFinishedSemaphores[currentFrame]; }

    void incrementeCurrentFrame();

private:
    VkCommandPool commandPool;
    
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<bool> commandBufferState;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> inFlightFences;

    std::vector<VkCommandBuffer> computeCommandBuffers;
    std::vector<VkSemaphore> computeFinishedSemaphores;
    std::vector<VkFence> computeInFlightFences;

    uint32_t currentFrame;    
};

#endif // VULKAN_RENDERER_HPP