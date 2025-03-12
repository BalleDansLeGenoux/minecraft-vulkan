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
    : pos(ppos), allocId(-1) {}

    void init();
    void addVoxel(glm::ivec3 pos, Voxel voxel);
    void updateMesh();
    void cleanup();

    glm::ivec3 getPos()             { return pos; }
    int        getAllocInfoOffset() { return allocId; }

private:
    glm::ivec3 pos;

    Voxel voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    Mesh mesh;

    int allocId;
};

#endif