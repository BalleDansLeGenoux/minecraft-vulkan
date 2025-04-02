#ifndef VULKAN_BUFFER_H
#define VULKAN_BUFFER_H

#include <vulkan/vulkan.h>
#include <vector>

#include <glm/glm.hpp>

struct Vertex;

class Buffer {
public:
    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    
    void cleanup();

    VkBuffer& getBuffer() { return buffer; }
    VkDeviceMemory& getBufferMemory() { return bufferMemory; }
    VkDeviceSize getSize() { return size; }

private:
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
    VkDeviceSize size;
};

#endif // BUFFER_MANAGER_HPP
