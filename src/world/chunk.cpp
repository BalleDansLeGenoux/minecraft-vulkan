#include "world/chunk.h"

#include <iostream>

#include "graphics/buffer_manager.h"
#include "world/chunk_manager.h"
#include "engine/voxel_data.h"

void Chunk::init() {
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                _voxels[x][y][z] = Voxel(0);
            }
        }
    }
}

void Chunk::addVoxel(glm::ivec3 p_pos, Voxel p_voxel) {
    _voxels[p_pos.x][p_pos.y][p_pos.z] = p_voxel;
    _is_modify = true;
}

void Chunk::update() {
    if (!_is_modify) return;

    Voxel voxel;
    CubeTextureData texture_coord;
    glm::vec3 cube_pos;
    float shininess;
    bool transparent;

    Chunk* chunk;
    
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                voxel = _voxels[x][y][z];
                if (voxel.id == 0) continue;

                cube_pos = {_pos.x*CHUNK_SIZE+x, _pos.y*CHUNK_SIZE+y, _pos.z*CHUNK_SIZE+z};

                texture_coord = TEXTURE_COORD.at(voxel.id-1);
                shininess = VOXEL_DATAS.at(voxel.id-1).shininess;
                transparent = VOXEL_DATAS.at(voxel.id-1).transparent;

                Mesh& mesh = transparent ? _transparent_mesh : _opaque_mesh;

                // bon je sais c'est moche mais c'est tout con, en gros chaque condition check si le block adjacent est vide ou pas et si il est en bord de chunk check si le chunk adjacent existe et si oui si le block adjacent du chunk adjacent est vide ou pas, si y'a rien a coté on ajoute la face sinon non, et on fait ça pour chaque face

                if ((x+1 < CHUNK_SIZE && (_voxels[x+1][y][z].id == 0 || (!VOXEL_DATAS[voxel.id-1].transparent && VOXEL_DATAS[_voxels[x+1][y][z].id-1].transparent))) || (x == CHUNK_SIZE-1 && (((chunk = ChunkManager::get().getChunk({_pos.x+1, _pos.y, _pos.z})) != nullptr && (chunk->getVoxel({0, y, z}).id == 0 || (!VOXEL_DATAS[voxel.id-1].transparent && VOXEL_DATAS[chunk->getVoxel({0, y, z}).id-1].transparent))) || chunk == nullptr))) mesh.add(cube_pos, PLUS_X, {1, 0, 0}, texture_coord.side, shininess);

                if ((x-1 >= 0 && (_voxels[x-1][y][z].id == 0 || (!VOXEL_DATAS[voxel.id-1].transparent && VOXEL_DATAS[_voxels[x-1][y][z].id-1].transparent))) || (x == 0 && (((chunk = ChunkManager::get().getChunk({_pos.x-1, _pos.y, _pos.z})) != nullptr && (chunk->getVoxel({CHUNK_SIZE-1, y, z}).id == 0 || (!VOXEL_DATAS[voxel.id-1].transparent && VOXEL_DATAS[chunk->getVoxel({CHUNK_SIZE-1, y, z}).id-1].transparent))) || chunk == nullptr))) mesh.add(cube_pos, MINUS_X, {-1, 0, 0}, texture_coord.side, shininess);

                // =====================================================================================================================================================

                if ((y+1 < CHUNK_SIZE && (_voxels[x][y+1][z].id == 0 || (!VOXEL_DATAS[voxel.id-1].transparent && VOXEL_DATAS[_voxels[x][y+1][z].id-1].transparent))) || (y == CHUNK_SIZE-1 && (((chunk = ChunkManager::get().getChunk({_pos.x, _pos.y+1, _pos.z})) != nullptr && (chunk->getVoxel({x, 0, z}).id == 0 || (!VOXEL_DATAS[voxel.id-1].transparent && VOXEL_DATAS[chunk->getVoxel({x, 0, z}).id-1].transparent))) || chunk == nullptr))) mesh.add(cube_pos, PLUS_Y, {0, 1, 0}, texture_coord.up, shininess);

                if ((y-1 >= 0 && (_voxels[x][y-1][z].id == 0 || (!VOXEL_DATAS[voxel.id-1].transparent && VOXEL_DATAS[_voxels[x][y-1][z].id-1].transparent))) || (y == 0 && (((chunk = ChunkManager::get().getChunk({_pos.x, _pos.y-1, _pos.z})) != nullptr && (chunk->getVoxel({x, CHUNK_SIZE-1, z}).id == 0 || (!VOXEL_DATAS[voxel.id-1].transparent && VOXEL_DATAS[chunk->getVoxel({x, CHUNK_SIZE-1, z}).id-1].transparent))) || chunk == nullptr))) mesh.add(cube_pos, MINUS_Y, {0, -1, 0}, texture_coord.down, shininess);
                
                // =====================================================================================================================================================

                if ((z+1 < CHUNK_SIZE && (_voxels[x][y][z+1].id == 0 || (!VOXEL_DATAS[voxel.id-1].transparent && VOXEL_DATAS[_voxels[x][y][z+1].id-1].transparent))) || (z == CHUNK_SIZE-1 && (((chunk = ChunkManager::get().getChunk({_pos.x, _pos.y, _pos.z+1})) != nullptr && (chunk->getVoxel({x, y, 0}).id == 0 || (!VOXEL_DATAS[voxel.id-1].transparent && VOXEL_DATAS[chunk->getVoxel({x, y, 0}).id-1].transparent))) || chunk == nullptr))) mesh.add(cube_pos, PLUS_Z, {0, 0, 1}, texture_coord.side, shininess);

                if ((z-1 >= 0 && (_voxels[x][y][z-1].id == 0 || (!VOXEL_DATAS[voxel.id-1].transparent && VOXEL_DATAS[_voxels[x][y][z-1].id-1].transparent))) || (z == 0 && (((chunk = ChunkManager::get().getChunk({_pos.x, _pos.y, _pos.z-1})) != nullptr && (chunk->getVoxel({x, y, CHUNK_SIZE-1}).id == 0 || (!VOXEL_DATAS[voxel.id-1].transparent && VOXEL_DATAS[chunk->getVoxel({x, y, CHUNK_SIZE-1}).id-1].transparent))) || chunk == nullptr))) mesh.add(cube_pos, MINUS_Z, {0, 0, -1}, texture_coord.side, shininess);
            }
        }
    }
}

void Chunk::upload() {
    if (! _opaque_mesh.isEmpty() & _is_modify) _opaque_alloc_id = BufferManager::get().getAllocator().allocMesh(_opaque_mesh, _opaque_alloc_id);
    if (! _transparent_mesh.isEmpty()) {
        // std::cout << "[DEBUG] " << _transparent_alloc_id << std::endl;
        _transparent_mesh.sort();
        _transparent_alloc_id = BufferManager::get().getTransparentAllocator().allocMesh(_transparent_mesh, _transparent_alloc_id);
    }

    _is_modify = false;
}

void Chunk::cleanup() {
    BufferManager::get().getAllocator().freeMesh(_opaque_alloc_id);
    BufferManager::get().getTransparentAllocator().freeMesh(_transparent_alloc_id);
}