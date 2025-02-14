#include "world/chunk.h"

#include "graphics/buffer_manager.h"

void Chunk::init() {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                voxels[x][y][z] = Voxel(0);
            }
        }
    }
}

void Chunk::addVoxel(glm::ivec3 pos, Voxel voxel) {
    voxels[pos.x][pos.y][pos.z] = voxel;
}

void Chunk::updateMesh() {
    mesh.cullingMeshing(pos, voxels);
    allocInfo = BufferManager::get().getAllocator().allocMesh(mesh);
}