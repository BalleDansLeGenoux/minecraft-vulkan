#include "graphics/buffer_manager.h"

#include <iostream>
#include <string.h>

#include "core/config.h"
#include "graphics/buffer.h"
#include "graphics/vertex.h"
#include "graphics/renderer.h"
#include "graphics/device.h"

void BufferManager::createBuffers() {
    allocator.init();

    VkDeviceSize bufferSize = SIZE_CHUNK_BUFFER;

    voxelBuffer.createBuffer(bufferSize,
                            VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
                            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    updateVoxelBuffer.createBuffer(bufferSize,
                                VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                                VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
                                VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );
}

void BufferManager::cleanupBuffers() {
    allocator.cleanup();
    voxelBuffer.cleanup();
    updateVoxelBuffer.cleanup();
}


void BufferManager::createUniformBuffers() {
    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        uniformBuffers[i].createUniformBuffer();
    }
}

void BufferManager::updateUniformBuffer(glm::mat4 matrix) {
    uniformBuffers.at(Renderer::get().getCurrentFrame()).updateUniformBuffer(matrix);
}

void BufferManager::cleanupUniformBuffer() {
    for (auto ubo : uniformBuffers) {
        ubo.cleanup();
    }
}

void BufferManager::updateUpdateVoxelBuffer(std::vector<BlockUpdate>& blockUpdate) {
    VkDeviceSize bufferSize = SIZE_CHUNK_BUFFER;

    Buffer stagingBuffer;
    stagingBuffer.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    void* data;
    vkMapMemory(Device::get().getDevice(), stagingBuffer.getBufferMemory(), 0, bufferSize, 0, &data);
        memcpy(data, blockUpdate.data(), (size_t) bufferSize);
    vkUnmapMemory(Device::get().getDevice(), stagingBuffer.getBufferMemory());

    copyBuffer(stagingBuffer.getBuffer(), updateVoxelBuffer.getBuffer(), bufferSize);

    stagingBuffer.cleanup();
}

void BufferManager::printVertexBuffer(size_t sizeToPrint) {
    VkDeviceSize bufferSize = sizeof(Vertex) * sizeToPrint;
    
    Buffer stagingBuffer;
    stagingBuffer.createBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    copyBuffer(BufferManager::get().getVertexBuffers().getBuffer(), stagingBuffer.getBuffer(), bufferSize);

    void* data;
    vkMapMemory(Device::get().getDevice(), stagingBuffer.getBufferMemory(), 0, bufferSize, 0, &data);
    Vertex* resultData = static_cast<Vertex*>(data);

    std::cout << "Vertex Buffer -> Byte print : " << bufferSize << std::endl;

    for (int i = 0; i < sizeToPrint; ++i) {
        std::cout << "  pos      [" << i << "] = " << resultData[i].pos.x << " | " << resultData[i].pos.y << " | " << resultData[i].pos.z << std::endl;
        // std::cout << "  color    [" << i << "] = " << resultData[i].normal.x << " | " << resultData[i].normal.y << " | " << resultData[i].normal.z << std::endl;
        // std::cout << "  texCoord [" << i << "] = " << resultData[i].texCoord.x << " | " << resultData[i].texCoord.y << std::endl << std::endl;
    }

    vkUnmapMemory(Device::get().getDevice(), stagingBuffer.getBufferMemory());
    
    stagingBuffer.cleanup();
}


void BufferManager::printIndexBuffer(size_t sizeToPrint) {
    VkDeviceSize bufferSize = sizeof(uint32_t) * sizeToPrint;
    
    Buffer stagingBuffer;
    stagingBuffer.createBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    copyBuffer(BufferManager::get().getIndexBuffers().getBuffer(), stagingBuffer.getBuffer(), bufferSize);

    void* data;
    vkMapMemory(Device::get().getDevice(), stagingBuffer.getBufferMemory(), 0, bufferSize, 0, &data);
    uint32_t* resultData = static_cast<uint32_t*>(data);

    std::cout << "Index Buffer -> Byte print : " << bufferSize << std::endl;

    for (int i = 0; i < sizeToPrint; ++i) {
        std::cout << "  index [" << i << "] = " << resultData[i] << std::endl;
    }

    vkUnmapMemory(Device::get().getDevice(), stagingBuffer.getBufferMemory());
    
    stagingBuffer.cleanup();
}


void BufferManager::printIndirectBuffer(size_t sizeToPrint) {
    VkDeviceSize bufferSize = sizeof(DrawIndirectCommand) * sizeToPrint;
    
    Buffer stagingBuffer;
    stagingBuffer.createBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    copyBuffer(BufferManager::get().getAllocator().getIndirectBuffer().getBuffer(), stagingBuffer.getBuffer(), bufferSize);

    void* data;
    vkMapMemory(Device::get().getDevice(), stagingBuffer.getBufferMemory(), 0, bufferSize, 0, &data);
    DrawIndirectCommand* resultData = static_cast<DrawIndirectCommand*>(data);

    std::cout << "Indirect Buffer -> Byte print : " << bufferSize << std::endl;

    for (int i = 0; i < sizeToPrint; ++i) {
        std::cout << "  DrawIndirectCommand [" << i << "].firstInstance = " << resultData[i].firstInstance << std::endl;
        std::cout << "  DrawIndirectCommand [" << i << "].indexCount = " << resultData[i].indexCount << std::endl;
        std::cout << "  DrawIndirectCommand [" << i << "].indexOffset = " << resultData[i].indexOffset << std::endl;
        std::cout << "  DrawIndirectCommand [" << i << "].vertexOffset = " << resultData[i].vertexOffset << std::endl;
        std::cout << "  DrawIndirectCommand [" << i << "].instanceCount = " << resultData[i].instanceCount << std::endl << std::endl;
    }

    vkUnmapMemory(Device::get().getDevice(), stagingBuffer.getBufferMemory());
    
    stagingBuffer.cleanup();
}

void BufferManager::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(Device::get().getDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(Device::get().getDevice(), image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(Device::get().getDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(Device::get().getDevice(), image, imageMemory, 0);
}

void BufferManager::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    copyBuffer(srcBuffer, dstBuffer, size, 0, 0);
}

void BufferManager::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size, VkDeviceSize srcOffset, VkDeviceSize dstOffset) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    copyRegion.srcOffset = srcOffset;
    copyRegion.dstOffset = dstOffset;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer);
}

VkCommandBuffer BufferManager::beginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = Renderer::get().getCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(Device::get().getDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void BufferManager::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(Device::get().getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(Device::get().getGraphicsQueue());

    vkFreeCommandBuffers(Device::get().getDevice(), Renderer::get().getCommandPool(), 1, &commandBuffer);
}

uint32_t BufferManager::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(Device::get().getPhysicalDevice(), &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}