#ifndef CHUNK_H
#define CHUNK_H

#include <glm/glm.hpp>

#include "core/config.h"
#include "engine/mesh.h"
#include "world/voxel.h"
#include "graphics/allocator.h"

class Chunk {
public:
    Chunk() {}
    
    Chunk(glm::ivec3 ppos)
    : pos(ppos) {}

    void init();
    void addVoxel(glm::ivec3 pos, Voxel voxel);
    void updateMesh();
private:
    glm::ivec3 pos;

    Voxel voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    Mesh mesh;

    DrawIndirectCommand allocInfo;
};

#endif