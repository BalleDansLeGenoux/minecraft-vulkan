#include "engine/mesh_builder.h"

#include "engine/voxel_data.h"
#include "engine/voxel_direction.h"
#include "world/chunk_manager.h"

void MeshBuilder::buildMeshes(Voxel (& p_voxels) [CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE], glm::ivec3 p_chunk_pos) {
    ChunkManager& cm = ChunkManager::get();

    Chunk* cxp = cm.getChunk({p_chunk_pos.x+1, p_chunk_pos.y,   p_chunk_pos.z});
    Chunk* cxm = cm.getChunk({p_chunk_pos.x-1, p_chunk_pos.y,   p_chunk_pos.z});
    Chunk* cyp = cm.getChunk({p_chunk_pos.x,   p_chunk_pos.y+1, p_chunk_pos.z});
    Chunk* cym = cm.getChunk({p_chunk_pos.x,   p_chunk_pos.y-1, p_chunk_pos.z});
    Chunk* czp = cm.getChunk({p_chunk_pos.x,   p_chunk_pos.y,   p_chunk_pos.z+1});
    Chunk* czm = cm.getChunk({p_chunk_pos.x,   p_chunk_pos.y,   p_chunk_pos.z-1});

    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                const Voxel voxel = p_voxels[x][y][z];
                if (voxel.id == 0) continue;

                const glm::vec3 cube_pos = {p_chunk_pos.x * CHUNK_SIZE + x, p_chunk_pos.y * CHUNK_SIZE + y, p_chunk_pos.z * CHUNK_SIZE + z};
                const VoxelData& data = VOXEL_DATAS[voxel.id - 1];
                const CubeTextureData& tex = TEXTURE_COORD[voxel.id - 1];
                Mesh& mesh = data.transparent ? _transparent_mesh : _opaque_mesh;

                auto shouldRender = [&](int nx, int ny, int nz, Chunk* neighborChunk) -> bool {
                    if (nx >= 0 && nx < CHUNK_SIZE &&
                        ny >= 0 && ny < CHUNK_SIZE &&
                        nz >= 0 && nz < CHUNK_SIZE) {
                        const Voxel n_voxel = p_voxels[nx][ny][nz];
                        if (n_voxel.id == 0) return true;
                        const VoxelData& n_data = VOXEL_DATAS[n_voxel.id - 1];
                        return !data.transparent && n_data.transparent;
                    } else if (neighborChunk) {
                        const Voxel n_voxel = neighborChunk->getVoxel({(nx + CHUNK_SIZE) % CHUNK_SIZE,
                                                                        (ny + CHUNK_SIZE) % CHUNK_SIZE,
                                                                        (nz + CHUNK_SIZE) % CHUNK_SIZE});
                        if (n_voxel.id == 0) return true;
                        const VoxelData& n_data = VOXEL_DATAS[n_voxel.id - 1];
                        return !data.transparent && n_data.transparent;
                    }
                    return true;
                };

                if (shouldRender(x + 1, y, z, cxp)) mesh.add(cube_pos, PLUS_X, {1, 0, 0}, tex.side, data.shininess);
                if (shouldRender(x - 1, y, z, cxm)) mesh.add(cube_pos, MINUS_X, {-1, 0, 0}, tex.side, data.shininess);
                if (shouldRender(x, y + 1, z, cyp)) mesh.add(cube_pos, PLUS_Y, {0, 1, 0}, tex.up,   data.shininess);
                if (shouldRender(x, y - 1, z, cym)) mesh.add(cube_pos, MINUS_Y, {0, -1, 0}, tex.down, data.shininess);
                if (shouldRender(x, y, z + 1, czp)) mesh.add(cube_pos, PLUS_Z, {0, 0, 1}, tex.side, data.shininess);
                if (shouldRender(x, y, z - 1, czm)) mesh.add(cube_pos, MINUS_Z, {0, 0, -1}, tex.side, data.shininess);
            }
        }
    }
}