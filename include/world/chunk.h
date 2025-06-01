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
    
    Chunk(glm::ivec3 p_pos)
    : _pos(p_pos), _opaque_alloc_id(-1), _transparent_alloc_id(-1) {}

    void init();
    void addVoxel(glm::ivec3 pos, Voxel voxel);
    void update();
    void upload();
    void cleanup();

    glm::ivec3 getPos()                      { return _pos; }
    int        getOpaqueAllocId()            { return _opaque_alloc_id; }
    int        getTransparentAllocId()       { return _transparent_alloc_id; }
    Voxel      getVoxel(int x, int y, int z) { return _voxels[x][y][z]; }
    Voxel      getVoxel(glm::vec3 p_pos)     { return getVoxel(p_pos.x, p_pos.y, p_pos.z); }

private:
    glm::ivec3 _pos;

    Voxel _voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    Mesh _opaque_mesh;
    Mesh _transparent_mesh;

    int _opaque_alloc_id;
    int _transparent_alloc_id;
};

#endif