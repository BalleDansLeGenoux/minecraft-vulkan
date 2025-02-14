#ifndef VULKAN_ALLOCATOR_H
#define VULKAN_ALLOCATOR_H

#include <cstdint>
#include <vector>
#include <glm/glm.hpp>

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

class Allocator {
public:
    void init();
    DrawIndirectCommand allocMesh(Mesh& mesh);

    void cleanup();

    Buffer& getVertexBuffer()   { return vertex; }
    Buffer& getIndexBuffer()    { return index; }
    Buffer& getIndirectBuffer() { return indirect; }
    int     getVertexOffset()   { return vertexOffset; }
    int     getIndexOffset()    { return indexOffset; }
    int     getIndirectOffset() { return indirectOffset; }
    // int     getNumberVertex()   { return vertexOffset; }
    int     getNumberIndex()    { return indexCount; }
    int     getNumberIndirect() { return indirectCount; }

    void debugOffset();

private:
    Buffer vertex;
    Buffer index;
    Buffer indirect;

    int vertexOffset;
    int indexOffset;
    int indirectOffset;
    int vertexCount;
    int indexCount;
    int indirectCount;

    void allocData(VkBuffer dstBuffer, void* pdata, size_t sizeData, int offset);
};

#endif