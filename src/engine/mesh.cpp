#include "engine/mesh.h"

#include <iostream>

#include "graphics/buffer_manager.h"
#include "engine/voxel_data.h"
#include "world/chunk.h"
#include "world/chunk_manager.h"

void Mesh::cullingMeshing(glm::vec3 chunkPos, Voxel voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]) {
    clear();

    CubeTextureData texture_coord;
    glm::vec3 cube_pos;
    float shininess;

    Chunk* chunk;
    
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                if (voxels[x][y][z].id == 0) continue;

                cube_pos = {chunkPos.x*CHUNK_SIZE+x, chunkPos.y*CHUNK_SIZE+y, chunkPos.z*CHUNK_SIZE+z};

                texture_coord = TEXTURE_COORD.at(voxels[x][y][z].id-1);
                shininess = VOXEL_DATAS.at(voxels[x][y][z].id-1).shininess;

                // bon je sais c'est moche mais c'est tout con, en gros chaque condition check si le block adjacent est vide ou pas et si il est en bord de chunk check si le chunk adjacent existe et si oui si le block adjacent du chunk adjacent est vide ou pas, si y'a rien a coté on ajoute la face sinon non, et on fait ça pour chaque face
                
                if ((x+1 < CHUNK_SIZE && voxels[x+1][y][z].id == 0) || (x == CHUNK_SIZE-1 && (((chunk = ChunkManager::get().getChunk({chunkPos.x+1, chunkPos.y, chunkPos.z})) != nullptr && chunk->getVoxel({0, y, z}).id == 0) || chunk == nullptr))) addQuad(cube_pos, PLUS_X, {1, 0, 0}, texture_coord.side, shininess);
                if ((x-1 >= 0 && voxels[x-1][y][z].id == 0) || (x == 0 && (((chunk = ChunkManager::get().getChunk({chunkPos.x-1, chunkPos.y, chunkPos.z})) != nullptr && chunk->getVoxel({CHUNK_SIZE-1, y, z}).id == 0) || chunk == nullptr))) addQuad(cube_pos, MINUS_X, {-1, 0, 0}, texture_coord.side, shininess);

                if ((y+1 < CHUNK_SIZE && voxels[x][y+1][z].id == 0) || (y == CHUNK_SIZE-1 && (((chunk = ChunkManager::get().getChunk({chunkPos.x, chunkPos.y+1, chunkPos.z})) != nullptr && chunk->getVoxel({x, 0, z}).id == 0) || chunk == nullptr))) addQuad(cube_pos, PLUS_Y, {0, 1, 0}, texture_coord.up, shininess);
                if ((y-1 >= 0 && voxels[x][y-1][z].id == 0) || (y == 0 && (((chunk = ChunkManager::get().getChunk({chunkPos.x, chunkPos.y-1, chunkPos.z})) != nullptr && chunk->getVoxel({x, CHUNK_SIZE-1, z}).id == 0) || chunk == nullptr))) addQuad(cube_pos, MINUS_Y, {0, -1, 0}, texture_coord.down, shininess);
                
                if ((z+1 < CHUNK_SIZE && voxels[x][y][z+1].id == 0) || (z == CHUNK_SIZE-1 && (((chunk = ChunkManager::get().getChunk({chunkPos.x, chunkPos.y, chunkPos.z+1})) != nullptr && chunk->getVoxel({x, y, 0}).id == 0) || chunk == nullptr))) addQuad(cube_pos, PLUS_Z, {0, 0, 1}, texture_coord.side, shininess);
                if ((z-1 >= 0 && voxels[x][y][z-1].id == 0) || (z == 0 && (((chunk = ChunkManager::get().getChunk({chunkPos.x, chunkPos.y, chunkPos.z-1})) != nullptr && chunk->getVoxel({x, y, CHUNK_SIZE-1}).id == 0) || chunk == nullptr))) addQuad(cube_pos, MINUS_Z, {0, 0, -1}, texture_coord.side, shininess);
            }
        }
    }
}

void Mesh::addQuad(glm::vec3 pos, FacePosition face_pos, glm::vec3 normal, FaceTextureData uv, float shininess) {
    addIndex();

    vertex.insert( vertex.end(), {
        {pos + face_pos.v[0], normal, uv.v[0], shininess},
        {pos + face_pos.v[1], normal, uv.v[1], shininess},
        {pos + face_pos.v[2], normal, uv.v[2], shininess},
        {pos + face_pos.v[3], normal, uv.v[3], shininess}}
    );
}

void Mesh::addIndex() {
    uint indexOffset = vertex.size();

    index.insert(index.end(), {
        indexOffset + 0,
        indexOffset + 1,
        indexOffset + 2,
        indexOffset + 2,
        indexOffset + 3,
        indexOffset + 0}
    );
}

void Mesh::clear() {
    vertex.clear();
    index.clear();
}