#ifndef BUFFER_MANAGER_HPP
#define BUFFER_MANAGER_HPP

#include <vulkan/vulkan.h>
#include <vector>

#include <glm/glm.hpp>

#include "core/vulkan_tools.h"

class VulkanDevice;
class VulkanSwapchain;
class VulkanPipeline;
class VulkanRenderer;

struct Vertex;

class VulkanBuffer {
public:
    void createVertexBuffer();
    void createIndexBuffer();
    void createVoxelBuffer();
    void createUpdateVoxelBuffer();
    void createUniformBuffers();
    void updateUniformBuffer(glm::mat4 matrix);
    void cleanupUniform();
    void cleanupVertexIndices();

    void     createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    void     createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void     copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    VkCommandBuffer beginSingleTimeCommands();
    void            endSingleTimeCommands(VkCommandBuffer commandBuffer);

    void updateVoxelsBuffer(std::vector<BlockUpdate>& blockUpdate);

    std::vector<VkBuffer> getUniformBuffers() const { return uniformBuffers; }

    const VkBuffer* getVertexBuffer()      const { return &vertexBuffer; }
    const VkBuffer* getIndexBuffer()       const { return &indexBuffer; }
    const VkBuffer* getVoxelBuffer()       const { return &voxelBuffer; }
    const VkBuffer* getUpdateVoxelBuffer() const { return &updateVoxelBuffer; }

    VulkanBuffer(VulkanDevice& pvulkanDevice, VulkanSwapchain& pvulkanSwapchain, VulkanPipeline& pvulkanPipeline, VulkanRenderer& pvulkanRenderer)
    : vulkanDevice(pvulkanDevice), vulkanSwapchain(pvulkanSwapchain), vulkanPipeline(pvulkanPipeline), vulkanRenderer(pvulkanRenderer) {}

private:
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    VkBuffer voxelBuffer;
    VkDeviceMemory voxelBufferMemory;

    VkBuffer updateVoxelBuffer;
    VkDeviceMemory updateVoxelBufferMemory;

    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    VulkanDevice& vulkanDevice;
    VulkanSwapchain& vulkanSwapchain;
    VulkanPipeline& vulkanPipeline;
    VulkanRenderer& vulkanRenderer;
};

#endif // BUFFER_MANAGER_HPP
