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
        VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
        VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    indirect.createBuffer(bufferSize,
        VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT |
        VK_BUFFER_USAGE_TRANSFER_DST_BIT |
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );

    vertexOffset = 0;
    indexOffset = 0;
    indirectOffset = 0;

    id = 0;
}

int  Allocator::allocMesh(Mesh& mesh, int pid) {
    auto pvertex = mesh.getVertex();
    auto pindex = mesh.getIndex();

    int currentVertexOffset = vertexOffset;
    int currentIndexOffset = indexOffset;
    int currentIndirectOffset = indirectOffset;
    
    DrawIndirectCommand allocInfo;
    allocInfo.indexCount = pindex.size();
    allocInfo.instanceCount = 1;
    allocInfo.firstInstance = 0;

    // Dans le cas ou le chunk est allouer pour la première fois on lui attribut un id
    int out = pid;
    if (out == -1) {
        out = id;
        id++;

        int allocBlockId = availableAlloc(pvertex.size());

        if (allocBlockId < 0) {
            int vertexMax = pvertex.size() * 1.5;
            int indexMax = pindex.size() * 1.5;
        
            used[out] = {
                static_cast<uint32_t>(vertexOffset),
                static_cast<uint32_t>(vertexMax),
                static_cast<uint32_t>(indexOffset),
                static_cast<uint32_t>(indexMax),
                static_cast<uint32_t>(indirectOffset)
            };

            allocInfo.indexOffset = indexOffset;
            allocInfo.vertexOffset = vertexOffset;

            vertexOffset += vertexMax;
            indexOffset += indexMax;
            indirectOffset++;
        } else {
            AllocInfo infos = freeList[allocBlockId];
            freeList.erase(freeList.begin()+allocBlockId);

            used[out] = infos;

            allocInfo.indexOffset = infos.indexOffset;
            allocInfo.vertexOffset = infos.vertexOffset;

            currentVertexOffset = infos.vertexOffset;
            currentIndexOffset = infos.indexOffset;
            currentIndirectOffset = infos.indirectOffset;
        }
    } else {
        AllocInfo infos = used[out];

        allocInfo.vertexOffset = infos.vertexOffset;
        allocInfo.indexOffset = infos.indexOffset;

        currentVertexOffset = infos.vertexOffset;
        currentIndexOffset = infos.indexOffset;
        currentIndirectOffset = infos.indirectOffset;
    }

    currentVertexOffset *= sizeof(Vertex);
    currentIndexOffset *= sizeof(uint32_t);
    currentIndirectOffset *= sizeof(DrawIndirectCommand);

    // Alloc Vertex

    VkDeviceSize size = pvertex.size() * sizeof(Vertex);
    allocData(vertex.getBuffer(), pvertex.data(), size, currentVertexOffset);

    // Alloc Index

    size = pindex.size() * sizeof(uint32_t);
    allocData(index.getBuffer(), pindex.data(), size, currentIndexOffset);

    // Alloc Indirect

    size = sizeof(DrawIndirectCommand);
    allocData(indirect.getBuffer(), &allocInfo, size, currentIndirectOffset);

    return out;
}

void Allocator::freeMesh(int pid) {
    AllocInfo infos = used[pid];

    VkDeviceSize size = sizeof(DrawIndirectCommand);
    
    int offset = infos.indirectOffset * sizeof(DrawIndirectCommand);

    DrawIndirectCommand data;

    extractData(&data, indirect.getBuffer(), size, offset);

    freeList.push_back(infos);

    DrawIndirectCommand allocInfo;
    allocInfo.indexCount = 0;
    allocInfo.instanceCount = 0;
    allocInfo.indexOffset = 0;
    allocInfo.vertexOffset = 0;
    allocInfo.firstInstance = 0;

    allocData(indirect.getBuffer(), &allocInfo, size, offset);

    used.erase(pid);
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

void Allocator::extractData(void* dstData, VkBuffer srcBuffer, size_t sizeData, int offset) {
    Buffer staging;
    staging.createBuffer(sizeData,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT |
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    BufferManager::copyBuffer(srcBuffer, staging.getBuffer(), sizeData, offset, 0);

    void* data;
    vkMapMemory(Device::get().getDevice(), staging.getBufferMemory(), 0, sizeData, 0, &data);
    memcpy(dstData, data, sizeData);
    vkUnmapMemory(Device::get().getDevice(), staging.getBufferMemory());

    staging.cleanup();
}

int Allocator::availableAlloc(int sizeVertex) {
    if (freeList.empty()) return -1;

    int index = 0;
    for (AllocInfo info : freeList) {
        if (sizeVertex * 1.2 < info.vertexMax) return index;

        index++;
    }

    return -1;
}

void Allocator::cleanup() {
    vertex.cleanup();
    index.cleanup();
    indirect.cleanup();
}

void Allocator::debugOffset() {
    std::cout << "Offset vertex   : " << vertexOffset << std::endl;
    std::cout << "Offset index    : " << indexOffset << std::endl;
    std::cout << "Offset indirect : " << indirectOffset << std::endl;
}