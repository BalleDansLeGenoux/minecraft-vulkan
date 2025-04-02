#include "graphics/buffer.h"

#include <cstring>
#include <chrono>
#include <stdexcept>

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "core/config.h"
#include "graphics/device.h" 
#include "graphics/renderer.h"
#include "graphics/swapchain.h" 
#include "graphics/buffer_manager.h"

void Buffer::createBuffer(VkDeviceSize psize, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = psize;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(Device::get().getDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(Device::get().getDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = BufferManager::findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(Device::get().getDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(Device::get().getDevice(), buffer, bufferMemory, 0);

    size = psize;
}

void Buffer::cleanup() {
    vkDestroyBuffer(Device::get().getDevice(), buffer, nullptr);
    vkFreeMemory(Device::get().getDevice(), bufferMemory, nullptr);
}