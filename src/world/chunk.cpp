#include "world/chunk.h"

#include <iostream>

#include "graphics/buffer_manager.h"
#include "world/chunk_manager.h"

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
    allocId = BufferManager::get().getAllocator().allocMesh(mesh, allocId);
    std::cout << "POS : " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
    std::cout << "ID  : " << allocId << std::endl << std::endl;
}

void Chunk::cleanup() {
    BufferManager::get().getAllocator().freeMesh(allocId);
    std::cout << "POS : " << pos.x << ", " << pos.y << ", " << pos.z << std::endl;
    std::cout << "ID  : " << allocId << std::endl;
}