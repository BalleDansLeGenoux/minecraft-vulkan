#include "core/vulkan_buffer.h"

#include <cstring>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "core/vulkan_config.h"
#include "core/vulkan_device.h" 
#include "core/vulkan_pipeline.h"
#include "core/vulkan_renderer.h"
#include "core/vulkan_swapchain.h" 

void VulkanBuffer::createVertexBuffer() {
    VkDeviceSize bufferSize = sizeof(Vertex) * BASE_BUFFER_CAPACITY;

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
}

void VulkanBuffer::createIndexBuffer() {
    VkDeviceSize bufferSize = sizeof(uint32_t) * BASE_BUFFER_CAPACITY;

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);
}

// void VulkanBuffer::createVertexBuffer() {
//     VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

//     VkBuffer stagingBuffer;
//     VkDeviceMemory stagingBufferMemory;
//     createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

//     void* data;
//     vkMapMemory(vulkanDevice.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
//         memcpy(data, vertices.data(), (size_t) bufferSize);
//     vkUnmapMemory(vulkanDevice.getDevice(), stagingBufferMemory);

//     createBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

//     copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

//     vkDestroyBuffer(vulkanDevice.getDevice(), stagingBuffer, nullptr);
//     vkFreeMemory(vulkanDevice.getDevice(), stagingBufferMemory, nullptr);
// }

// void VulkanBuffer::createIndexBuffer() {
//     VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

//     VkBuffer stagingBuffer;
//     VkDeviceMemory stagingBufferMemory;
//     createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

//     void* data;
//     vkMapMemory(vulkanDevice.getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
//         memcpy(data, indices.data(), (size_t) bufferSize);
//     vkUnmapMemory(vulkanDevice.getDevice(), stagingBufferMemory);

//     createBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

//     copyBuffer(stagingBuffer, indexBuffer, bufferSize);

//     vkDestroyBuffer(vulkanDevice.getDevice(), stagingBuffer, nullptr);
//     vkFreeMemory(vulkanDevice.getDevice(), stagingBufferMemory, nullptr);
// }

void VulkanBuffer::createVoxelBuffer() {
    VkDeviceSize bufferSize = sizeof(uint) * CHUNK_HEIGHT * CHUNK_WIDTH * CHUNK_WIDTH * RENDER_DISTANCE * RENDER_DISTANCE * 4;

    createBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, voxelBuffer, voxelBufferMemory);
}

void VulkanBuffer::createUpdateVoxelBuffer() {
    VkDeviceSize bufferSize = sizeof(BlockUpdate) * CHUNK_HEIGHT * CHUNK_WIDTH * CHUNK_WIDTH * RENDER_DISTANCE * RENDER_DISTANCE * 4;

    createBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, updateVoxelBuffer, updateVoxelBufferMemory);
}

void VulkanBuffer::createUniformBuffers() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

        vkMapMemory(vulkanDevice.getDevice(), uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }
}

void VulkanBuffer::updateUniformBuffer() {
    static auto startTime = std::chrono::high_resolution_clock::now();

    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), vulkanSwapchain.getSwapChainExtent().width / (float) vulkanSwapchain.getSwapChainExtent().height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1;

    memcpy(uniformBuffersMapped[vulkanRenderer.getCurrentFrame()], &ubo, sizeof(ubo));
}

void VulkanBuffer::cleanupVertexIndices() {
    vkDestroyBuffer(vulkanDevice.getDevice(), indexBuffer, nullptr);
    vkFreeMemory(vulkanDevice.getDevice(), indexBufferMemory, nullptr);

    vkDestroyBuffer(vulkanDevice.getDevice(), vertexBuffer, nullptr);
    vkFreeMemory(vulkanDevice.getDevice(), vertexBufferMemory, nullptr);

    vkDestroyBuffer(vulkanDevice.getDevice(), voxelBuffer, nullptr);
    vkFreeMemory(vulkanDevice.getDevice(), voxelBufferMemory, nullptr);

    vkDestroyBuffer(vulkanDevice.getDevice(), updateVoxelBuffer, nullptr);
    vkFreeMemory(vulkanDevice.getDevice(), updateVoxelBufferMemory, nullptr);
}

void VulkanBuffer::cleanupUniform() {
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(vulkanDevice.getDevice(), uniformBuffers[i], nullptr);
        vkFreeMemory(vulkanDevice.getDevice(), uniformBuffersMemory[i], nullptr);
    }
}

void VulkanBuffer::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {
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

    if (vkCreateImage(vulkanDevice.getDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(vulkanDevice.getDevice(), image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(vulkanDevice.getDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(vulkanDevice.getDevice(), image, imageMemory, 0);
}

void VulkanBuffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(vulkanDevice.getDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(vulkanDevice.getDevice(), buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(vulkanDevice.getDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(vulkanDevice.getDevice(), buffer, bufferMemory, 0);
}

void VulkanBuffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer);
}

VkCommandBuffer VulkanBuffer::beginSingleTimeCommands() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = vulkanRenderer.getCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(vulkanDevice.getDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void VulkanBuffer::endSingleTimeCommands(VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(*vulkanDevice.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(*vulkanDevice.getGraphicsQueue());

    vkFreeCommandBuffers(vulkanDevice.getDevice(), vulkanRenderer.getCommandPool(), 1, &commandBuffer);
}

uint32_t VulkanBuffer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(vulkanDevice.getPhysicalDevice(), &memProperties);

        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }

        throw std::runtime_error("failed to find suitable memory type!");
    }