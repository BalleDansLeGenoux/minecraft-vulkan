#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H

#include "engine/voxel_data.h"
#include "engine/sortable_mesh.h"
#include "world/chunk_manager.h"

struct MeshBuilderContext {
    Voxel (& voxels) [CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
    ChunkManager& chunk_manager;
    glm::ivec3 chunk_pos;

    glm::ivec3 current_voxel_pos;

    glm::ivec3 direction;
    glm::ivec3 neighbor_chunk_voxel_pos;

    Voxel current_voxel;
    Voxel neighbor_voxel;

    VoxelData current_voxel_data;
    VoxelData neighbor_voxel_data;

    Chunk* neighbor_chunk;
};

class MeshBuilder {
public:
    void buildMeshes(Voxel (& voxels) [CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE], glm::ivec3 p_chunk_pos);

    Mesh& getOpaqueMesh() { return _opaque_mesh; }
    SortableMesh& getTransparentMesh() { return _transparent_mesh; }

private:
    Mesh _opaque_mesh;
    SortableMesh _transparent_mesh;

    bool isInsideChunk(glm::ivec3 p_pos);
    bool checkNeighbor(MeshBuilderContext& p_context);
};

#endif