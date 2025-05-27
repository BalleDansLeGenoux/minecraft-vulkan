#include "graphics/allocators_manager.h"

#include <iostream>
#include <string.h>
#include <vulkan/vulkan.h>

#include "core/config.h"
#include "graphics/device.h"
#include "graphics/buffer_manager.h"
#include "engine/mesh.h"

void AllocatorManager::init() {
    _staging.createBuffer(300000000,
        VK_BUFFER_USAGE_TRANSFER_DST_BIT |
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
        VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );

    uint32_t nbBlock = NB_FACE_CHUNK * std::pow(RENDER_DISTANCE, 2);

    _vertexAllocator = Allocator(
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
        nbBlock,
        static_cast<uint32_t>(NB_VERTEX_PER_BLOCK * sizeof(Vertex)),
        _staging
    );

    _indexAllocator = Allocator(
        VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT,
        nbBlock,
        static_cast<uint32_t>(NB_INDEX_PER_BLOCK * sizeof(uint32_t)),
        _staging
    );

    nbBlock = std::pow(RENDER_DISTANCE, 3);

    _indirectAllocator = Allocator(
        VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        nbBlock,
        static_cast<uint32_t>(sizeof(DrawIndirectCommand)),
        _staging
    );

    _id = 0;
    _nbDataBlock = 0;
    _nbIndirectBlock = 0;
    _stagingOffset = 0;
}

int  AllocatorManager::allocMesh(Mesh& mesh, int pid) {
    auto& vertex = mesh.getVertex();
    auto& index = mesh.getIndex();
    uint32_t nbBlock = static_cast<uint32_t>(vertex.size()) / NB_VERTEX_PER_BLOCK;
    uint32_t maxNbBlock = nbBlock * MARGIN_BLOCKS;
    
    DrawIndirectCommand indirectCommand;
    indirectCommand.indexCount = index.size();
    indirectCommand.instanceCount = 1;
    indirectCommand.firstInstance = 0;

    AllocInfo infos;

    // Dans le cas ou le chunk est allouer pour la première fois on lui attribut un id
    int out = pid;
    if (out == -1) {
        // Dans le cas le chunk n'a pas encore été allouer

        if (_freeId.empty()) {
            out = _id;
            _id++;
        } else {
            out = _freeId[0];
            _freeId.erase(_freeId.begin());
        }

        newAlloc(nbBlock, maxNbBlock, infos, out);
    } else {
        // Dans le cas ou le chunk est deja alloue en memoire
        
        infos = _used[out];

        if (nbBlock > infos.maxDataBlock) {
            _used.erase(out);
            _freeList.push_back(infos);

            freeIndirectBlock(infos.indirectBlock);

            newAlloc(nbBlock, maxNbBlock, infos, out);
        }
    }

    indirectCommand.vertexOffset = static_cast<uint32_t>(infos.dataBlock * NB_VERTEX_PER_BLOCK);
    indirectCommand.indexOffset = static_cast<uint32_t>(infos.dataBlock * NB_INDEX_PER_BLOCK);

    _vertexAllocator.alloc(vertex.data(), nbBlock, _stagingOffset, infos.dataBlock);
    _stagingOffset += nbBlock * _vertexAllocator.getBlockSize();

    _indexAllocator.alloc(index.data(), nbBlock, _stagingOffset, infos.dataBlock);
    _stagingOffset += nbBlock * _indexAllocator.getBlockSize();

    _indirectAllocator.alloc(&indirectCommand, 1, _stagingOffset, infos.indirectBlock);
    _stagingOffset += _indirectAllocator.getBlockSize();

    return out;
}

void AllocatorManager::freeMesh(int pid) {
    AllocInfo infos = _used[pid];

    _freeList.push_back(infos);

    freeIndirectBlock(infos.indirectBlock);

    _used.erase(pid);

    _freeId.push_back(pid);
}

int AllocatorManager::availableAlloc(uint32_t p_nbBlock) {
    if (_freeList.empty()) return -1;

    uint32_t index = 0;
    for (AllocInfo info : _freeList) {
        if (p_nbBlock * MARGIN_BLOCKS < info.maxDataBlock) return index;

        index++;
    }

    return -1;
}

void AllocatorManager::newAlloc(uint32_t p_nbBlock, uint32_t p_maxNbBlock, AllocInfo& p_infos, int p_id) {
    int allocBlockId = availableAlloc(p_nbBlock);

    if (allocBlockId < 0) {
        // Dans le cas ou la freelist ne contient pas de block valide

        p_infos = {
            static_cast<uint32_t>(_nbDataBlock),
            static_cast<uint32_t>(p_maxNbBlock),
            static_cast<uint32_t>(_nbIndirectBlock)
        };
    
        _used[p_id] = p_infos;
        
        _nbDataBlock += p_maxNbBlock;
        _nbIndirectBlock++;
    } else {
        // Dans le cas ou la freelist contient un block valide pour le chunk

        p_infos = _freeList[allocBlockId];
        _freeList.erase(_freeList.begin()+allocBlockId);

        _used[p_id] = p_infos;
    }
}

void AllocatorManager::freeIndirectBlock(uint32_t p_offset) {
    DrawIndirectCommand indirectCommand;
    indirectCommand.indexCount = 0;
    indirectCommand.instanceCount = 0;
    indirectCommand.indexOffset = 0;
    indirectCommand.vertexOffset = 0;
    indirectCommand.firstInstance = 0;

    _indirectAllocator.alloc(&indirectCommand, 1, _stagingOffset, p_offset);

    _stagingOffset += _indirectAllocator.getBlockSize();
}

void AllocatorManager::cleanup() {
    _vertexAllocator.cleanup();
    _indexAllocator.cleanup();
    _indirectAllocator.cleanup();
    _staging.cleanup();
}