#include "engine/mesh.h"

#include <iostream>

#include "graphics/buffer_manager.h"

void Mesh::addIndex() {
    int indexOffset = vertex.size();

    index.push_back(indexOffset + 0);
    index.push_back(indexOffset + 1);
    index.push_back(indexOffset + 2);
    index.push_back(indexOffset + 2);
    index.push_back(indexOffset + 3);
    index.push_back(indexOffset + 0);
}

// A modif pour compacter

void Mesh::addUpQuad(glm::vec3 pos, glm::vec3 normal, glm::vec2 uv) {
    vertex.push_back({pos + glm::vec3(0, 1, 0), normal, {0.0f, 0.0f}});
    vertex.push_back({pos + glm::vec3(1, 1, 0), normal, {1.0f/16, 0.0f}});
    vertex.push_back({pos + glm::vec3(1, 1, 1), normal, {1.0f/16, 1.0f/16}});
    vertex.push_back({pos + glm::vec3(0, 1, 1), normal, {0.0f, 1.0f/16}});
}

void Mesh::addDownQuad(glm::vec3 pos, glm::vec3 normal, glm::vec2 uv) {
    vertex.push_back({pos + glm::vec3(1, 0, 0), normal, {1.0f/16*2, 0.0f}});
    vertex.push_back({pos + glm::vec3(0, 0, 0), normal, {1.0f/16*3, 0.0f}});
    vertex.push_back({pos + glm::vec3(0, 0, 1), normal, {1.0f/16*3, 1.0f/16}});
    vertex.push_back({pos + glm::vec3(1, 0, 1), normal, {1.0f/16*2, 1.0f/16}});
}

void Mesh::addRightQuad(glm::vec3 pos, glm::vec3 normal, glm::vec2 uv) {
    vertex.push_back({pos + glm::vec3(1, 1, 1), normal, {1.0f/16*(2+1), 0.0f}});
    vertex.push_back({pos + glm::vec3(1, 1, 0), normal, {1.0f/16*(3+1), 0.0f}});
    vertex.push_back({pos + glm::vec3(1, 0, 0), normal, {1.0f/16*(3+1), 1.0f/16}});
    vertex.push_back({pos + glm::vec3(1, 0, 1), normal, {1.0f/16*(2+1), 1.0f/16}});
}

void Mesh::addLeftQuad(glm::vec3 pos, glm::vec3 normal, glm::vec2 uv) {
    vertex.push_back({pos + glm::vec3(0, 1, 0), normal, {1.0f/16*(2+1), 0.0f}});
    vertex.push_back({pos + glm::vec3(0, 1, 1), normal, {1.0f/16*(3+1), 0.0f}});
    vertex.push_back({pos + glm::vec3(0, 0, 1), normal, {1.0f/16*(3+1), 1.0f/16}});
    vertex.push_back({pos + glm::vec3(0, 0, 0), normal, {1.0f/16*(2+1), 1.0f/16}});
}

void Mesh::addFrontQuad(glm::vec3 pos, glm::vec3 normal, glm::vec2 uv) {
    vertex.push_back({pos + glm::vec3(0, 1, 1), normal, {1.0f/16*(2+1), 0.0f}});
    vertex.push_back({pos + glm::vec3(1, 1, 1), normal, {1.0f/16*(3+1), 0.0f}});
    vertex.push_back({pos + glm::vec3(1, 0, 1), normal, {1.0f/16*(3+1), 1.0f/16}});
    vertex.push_back({pos + glm::vec3(0, 0, 1), normal, {1.0f/16*(2+1), 1.0f/16}});
}

void Mesh::addBackQuad(glm::vec3 pos, glm::vec3 normal, glm::vec2 uv) {
    vertex.push_back({pos + glm::vec3(1, 1, 0), normal, {1.0f/16*(2+1), 0.0f}});
    vertex.push_back({pos + glm::vec3(0, 1, 0), normal, {1.0f/16*(3+1), 0.0f}});
    vertex.push_back({pos + glm::vec3(0, 0, 0), normal, {1.0f/16*(3+1), 1.0f/16}});
    vertex.push_back({pos + glm::vec3(1, 0, 0), normal, {1.0f/16*(2+1), 1.0f/16}});
}

void Mesh::cullingMeshing(glm::vec3 chunkPos, Voxel voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]) {
    clear();
    
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                if (voxels[x][y][z].id == 0) continue;

                if ((x+1 < CHUNK_SIZE && voxels[x+1][y][z].id == 0) || x == CHUNK_SIZE-1 ) {
                    addIndex();
                    addRightQuad({chunkPos.x*CHUNK_SIZE+x, chunkPos.y*CHUNK_SIZE+y, chunkPos.z*CHUNK_SIZE+z}, {1, 0, 0}, {0, 0});
                }
                
                if ((x-1 >= 0 && voxels[x-1][y][z].id == 0) || x == 0) {
                    addIndex();
                    addLeftQuad({chunkPos.x*CHUNK_SIZE+x, chunkPos.y*CHUNK_SIZE+y, chunkPos.z*CHUNK_SIZE+z}, {-1, 0, 0}, {0, 0});
                }
                
                if ((y+1 < CHUNK_SIZE && voxels[x][y+1][z].id == 0) || y == CHUNK_SIZE-1) {
                    addIndex();
                    addUpQuad({chunkPos.x*CHUNK_SIZE+x, chunkPos.y*CHUNK_SIZE+y, chunkPos.z*CHUNK_SIZE+z}, {0, 1, 0}, {0, 0});
                }
                
                if ((y-1 >= 0 && voxels[x][y-1][z].id == 0) || y == 0) {
                    addIndex();
                    addDownQuad({chunkPos.x*CHUNK_SIZE+x, chunkPos.y*CHUNK_SIZE+y, chunkPos.z*CHUNK_SIZE+z}, {0, -1, 0}, {0, 0});
                }

                if ((z+1 < CHUNK_SIZE && voxels[x][y][z+1].id == 0) || z == CHUNK_SIZE-1) {
                    addIndex();
                    addFrontQuad({chunkPos.x*CHUNK_SIZE+x, chunkPos.y*CHUNK_SIZE+y, chunkPos.z*CHUNK_SIZE+z}, {0, 0, 1}, {0, 0});
                }

                if ((z-1 >= 0 && voxels[x][y][z-1].id == 0) || z == 0) {
                    addIndex();
                    addBackQuad({chunkPos.x*CHUNK_SIZE+x, chunkPos.y*CHUNK_SIZE+y, chunkPos.z*CHUNK_SIZE+z}, {0, 0, -1}, {0, 0});
                }
            }
        }
    }
}

void Mesh::clear() {
    vertex.clear();
    index.clear();
}

void Mesh::addQuad(glm::vec3 pos, glm::vec3 normal, glm::vec2 uv) {
    int indexOffset = vertex.size();

    vertex.push_back({pos + glm::vec3(0, 1, 0), normal, {0.0f, 0.0f}});
    vertex.push_back({pos + glm::vec3(1, 1, 0), normal, {1.0f/16, 0.0f}});
    vertex.push_back({pos + glm::vec3(1, 1, 1), normal, {1.0f/16, 1.0f/16}});
    vertex.push_back({pos + glm::vec3(0, 1, 1), normal, {0.0f, 1.0f/16}});

    index.push_back(indexOffset + 0);
    index.push_back(indexOffset + 1);
    index.push_back(indexOffset + 2);
    index.push_back(indexOffset + 2);
    index.push_back(indexOffset + 3);
    index.push_back(indexOffset + 0);
}