#ifndef BUFFER_MANAGER_HPP
#define BUFFER_MANAGER_HPP

#include <vulkan/vulkan.h>
#include <vector>
#include <cstdint>

class VulkanDevice;
class VulkanSwapchain;
class VulkanPipeline;
class VulkanRenderer;

struct Vertex;

class VulkanBuffer {
public:
    void createVertexBuffer();
    void createIndexBuffer();
    void createUniformBuffers();
    void updateUniformBuffer();
    void cleanup();

    void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    VkCommandBuffer beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    std::vector<VkBuffer> getUniformBuffers() const { return uniformBuffers; }

    const VkBuffer* getVertexBuffer() const { return &vertexBuffer; }
    const VkBuffer* getIndexBuffer() const { return &indexBuffer; }

    VulkanBuffer(VulkanDevice& pvulkanDevice, VulkanSwapchain& pvulkanSwapchain, VulkanPipeline& pvulkanPipeline, VulkanRenderer& pvulkanRenderer)
    : vulkanDevice(pvulkanDevice), vulkanSwapchain(pvulkanSwapchain), vulkanPipeline(pvulkanPipeline), vulkanRenderer(pvulkanRenderer) {}

private:
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    VulkanDevice& vulkanDevice;
    VulkanSwapchain& vulkanSwapchain;
    VulkanPipeline& vulkanPipeline;
    VulkanRenderer& vulkanRenderer;
};

#endif // BUFFER_MANAGER_HPP
