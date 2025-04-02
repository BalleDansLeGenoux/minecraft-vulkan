#ifndef VULKAN_BUFFER_MANAGER_H
#define VULKAN_BUFFER_MANAGER_H

#include <vulkan/vulkan.h>
#include <vector>

#include "engine/mesh.h"
#include "graphics/uniform_buffer.h"
#include "graphics/block_update.h"
#include "graphics/allocators_manager.h"

class Buffer;

class BufferManager {
public:
    static BufferManager& get() {
        static BufferManager instance;
        return instance;
    }

    void createBuffers();
    void cleanupBuffers();

    void createUniformBuffers();
    void updateUniformBuffer(glm::mat4 matrix);
    void cleanupUniformBuffer();

    void updateUpdateVoxelBuffer(std::vector<BlockUpdate>& blockUpdate); // tmp
    void printVertexBuffer(size_t size);
    void printIndexBuffer(size_t size);
    void printIndirectBuffer(size_t sizeToPrint);

    static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
    static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset);
    static VkCommandBuffer beginSingleTimeCommands();
    static void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    static void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
    static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);


    Buffer& getVertexBuffers()     { return allocator.getVertexBuffer(); }
    Buffer& getIndexBuffers()      { return allocator.getIndexBuffer(); }
    Buffer& getVoxelBuffer()       { return voxelBuffer; }
    Buffer& getUpdateVoxelBuffer() { return updateVoxelBuffer; }
    AllocatorManager& getAllocator()      { return allocator; }
    UniformBuffer& getUniformBuffer(int i)   { return uniformBuffers[i]; }


private:
    AllocatorManager allocator;
    Buffer voxelBuffer;
    Buffer updateVoxelBuffer;
    std::vector<UniformBuffer> uniformBuffers;
};

#endif