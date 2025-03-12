#ifndef VULKAN_ALLOCATOR_H
#define VULKAN_ALLOCATOR_H

#include <cstdint>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <unordered_map>

#include "graphics/buffer.h"
#include "graphics/vertex.h"
#include "engine/mesh.h"

struct DrawIndirectCommand {
    uint32_t indexCount;    // Nombre d'indices à dessiner
    uint32_t instanceCount; // Toujours 1 pour du voxel
    uint32_t indexOffset;   // Offset dans l’index buffer
    uint32_t vertexOffset;  // Offset dans le vertex buffer
    uint32_t firstInstance; // Toujours 0 dans ce cas
};

struct AllocInfo {
    uint32_t vertexOffset;
    uint32_t vertexMax;
    uint32_t indexOffset;
    uint32_t indexMax;
    uint32_t indirectOffset;
};

class Allocator {
public:
    void init();
    int  allocMesh(Mesh& mesh, int pid);
    void freeMesh(int pid);

    void cleanup();

    Buffer& getVertexBuffer()   { return vertex; }
    Buffer& getIndexBuffer()    { return index; }
    Buffer& getIndirectBuffer() { return indirect; }
    
    int     getVertexCount()    { return vertexOffset; }
    int     getIndexCount()     { return indexOffset; }
    int     getIndirectCount()  { return indirectOffset; }

    std::vector<AllocInfo>& getFreeList() { return freeList; }

    void debugOffset();

private:
    Buffer vertex;
    Buffer index;
    Buffer indirect;

    uint32_t vertexOffset;
    uint32_t indexOffset;
    uint32_t indirectOffset;

    int id;

    std::unordered_map<u_int32_t, AllocInfo> used;
    std::vector<AllocInfo> freeList;

    void allocData(VkBuffer dstBuffer, void* pdata, size_t sizeData, int offset);
    void extractData(void* dstData, VkBuffer srcBuffer, size_t sizeData, int offset);
    int availableAlloc(int sizeVertex);
};

#endif