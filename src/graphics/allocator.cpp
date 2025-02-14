#include "graphics/allocator.h"

#include <iostream>
#include <string.h>
#include <vulkan/vulkan.h>

#include "core/config.h"
#include "graphics/device.h"
#include "graphics/buffer_manager.h"
#include "engine/mesh.h"

void Allocator::init() {
    VkDeviceSize bufferSize = SIZE_CHUNK_BUFFER * 1024;

    vertex.createBuffer(bufferSize,
                            VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
                            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT |
                            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    index.createBuffer(bufferSize,
                            VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                            VK_BUFFER_USAGE_TRANSFER_SRC_BIT |
                            VK_BUFFER_USAGE_INDEX_BUFFER_BIT |
                            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    vertexOffset = 0;
    indexOffset = 0;
    indexCount = 0;
}

DrawIndirectCommand Allocator::allocMesh(Mesh& mesh) {
    // a modif pour vkCmdDrawIndexedIndirect
    auto pvertex = mesh.getVertex();
    auto pindex = mesh.getIndex();

    for (uint32_t& data : pindex) {
        data += indexCount/1.5;
    }

    DrawIndirectCommand allocInfo;
    allocInfo.indexCount = pindex.size();
    allocInfo.instanceCount = 1;
    allocInfo.indexOffset = indexOffset;
    allocInfo.vertexOffset = vertexOffset;
    allocInfo.firstInstance = 0;

    // Alloc Vertex

    VkDeviceSize size = pvertex.size() * sizeof(Vertex);

    allocData(vertex.getBuffer(), pvertex.data(), size, vertexOffset);

    vertexOffset += size;

    // Alloc Index

    size = pindex.size() * sizeof(uint32_t);

    allocData(index.getBuffer(), pindex.data(), size, indexOffset);

    indexOffset += size;
    indexCount += pindex.size();

    return allocInfo;
}

void Allocator::allocData(VkBuffer dstBuffer, void* pdata, size_t sizeData, int offset) {
    Buffer staging;
    staging.createBuffer(sizeData,
                        VK_BUFFER_USAGE_TRANSFER_DST_BIT |
                        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    void* data;
    vkMapMemory(Device::get().getDevice(), staging.getBufferMemory(), 0, sizeData, 0, &data);
    memcpy(data, pdata, sizeData);
    vkUnmapMemory(Device::get().getDevice(), staging.getBufferMemory());

    BufferManager::copyBuffer(staging.getBuffer(), dstBuffer, sizeData, 0, offset);

    staging.cleanup();
}

void Allocator::cleanup() {
    vertex.cleanup();
    index.cleanup();
}