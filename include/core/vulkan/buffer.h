#ifndef VULKAN_BUFFER_H
#define VULKAN_BUFFER_H

#include <vulkan/vulkan.h>
#include <vector>

#include <glm/glm.hpp>

struct Vertex;

class Buffer {
public:
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);

    static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    static VkCommandBuffer beginSingleTimeCommands();
    static void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    static void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
    
    void cleanup();

    const VkBuffer& getBuffer() const { return buffer; }
    const VkDeviceMemory& getBufferMemory() const { return bufferMemory; }

private:
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
};

#endif // BUFFER_MANAGER_HPP
