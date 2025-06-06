#include "world/chunk.h"

#include <iostream>

#include "graphics/buffer_manager.h"
#include "world/chunk_manager.h"
#include "engine/voxel_data.h"
#include "engine/mesh_builder.h"

void Chunk::init() {
    _mesh_builder = new MeshBuilder;

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

    _mesh_builder->buildMeshes(_voxels, _pos);
}

void Chunk::upload() {
    Mesh& opaque_mesh = _mesh_builder->getOpaqueMesh();
    SortableMesh& transparent_mesh = _mesh_builder->getTransparentMesh();
    if (! opaque_mesh.isEmpty() & _is_modify) _opaque_alloc_id = BufferManager::get().getAllocator().allocMesh(opaque_mesh, _opaque_alloc_id);
    if (! transparent_mesh.isEmpty()) {
        transparent_mesh.sort();
        _transparent_alloc_id = BufferManager::get().getTransparentAllocator().allocMesh(transparent_mesh, _transparent_alloc_id);
    }

    _is_modify = false;
}

void Chunk::cleanup() {
    delete _mesh_builder;
    
    BufferManager::get().getAllocator().freeMesh(_opaque_alloc_id);
    BufferManager::get().getTransparentAllocator().freeMesh(_transparent_alloc_id);
}