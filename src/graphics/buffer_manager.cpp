#include "graphics/buffer_manager.h"

#include <iostream>
#include <string.h>

#include "graphics/buffer.h"
#include "graphics/vertex.h"
#include "graphics/config.h"
#include "graphics/renderer.h"
#include "graphics/device.h"

void BufferManager::createVertexBuffer() {
    VkDeviceSize bufferSize = SIZE_CHUNK_BUFFER * 1024;

    vertexBuffer.createBuffer(bufferSize,
                            VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
                            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
                            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );
}

void BufferManager::createIndexBuffer() {
    VkDeviceSize bufferSize = SIZE_CHUNK_BUFFER * 1024;

    indexBuffer.createBuffer(bufferSize,
                            VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
                            VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
                            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );
}

void BufferManager::createComputeBuffer() {
    VkDeviceSize bufferSize = SIZE_CHUNK_BUFFER;

    voxelBuffer.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    updateVoxelBuffer.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

}

void BufferManager::cleanupBuffers() {
    vertexBuffer.cleanup();
    indexBuffer.cleanup();
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

    stagingBuffer.copyBuffer(stagingBuffer.getBuffer(), updateVoxelBuffer.getBuffer(), bufferSize);

    stagingBuffer.cleanup();
}

void BufferManager::printVertexBuffer(size_t sizeToPrint) {
    VkDeviceSize bufferSize = sizeof(Vertex) * sizeToPrint;
    
    Buffer stagingBuffer;
    stagingBuffer.createBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    stagingBuffer.copyBuffer(BufferManager::get().getVertexBuffers().getBuffer(), stagingBuffer.getBuffer(), bufferSize);

    void* data;
    vkMapMemory(Device::get().getDevice(), stagingBuffer.getBufferMemory(), 0, bufferSize, 0, &data);
    Vertex* resultData = static_cast<Vertex*>(data);

    std::cout << "Vertex Buffer -> Byte print : " << bufferSize << std::endl;

    for (int i = 0; i < sizeToPrint; ++i) {
        std::cout << "  pos      [" << i << "] = " << resultData[i].pos.x << " | " << resultData[i].pos.y << " | " << resultData[i].pos.z << std::endl;
        std::cout << "  color    [" << i << "] = " << resultData[i].color.x << " | " << resultData[i].color.y << " | " << resultData[i].color.z << std::endl;
        std::cout << "  texCoord [" << i << "] = " << resultData[i].texCoord.x << " | " << resultData[i].texCoord.y << std::endl << std::endl;
    }

    vkUnmapMemory(Device::get().getDevice(), stagingBuffer.getBufferMemory());
    
    stagingBuffer.cleanup();
}


void BufferManager::printIndexBuffer(size_t sizeToPrint) {
    VkDeviceSize bufferSize = sizeof(uint32_t) * sizeToPrint;
    
    Buffer stagingBuffer;
    stagingBuffer.createBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    stagingBuffer.copyBuffer(BufferManager::get().getIndexBuffers().getBuffer(), stagingBuffer.getBuffer(), bufferSize);

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