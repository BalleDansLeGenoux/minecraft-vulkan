#include "engine/mesh_builder.h"

#include "engine/voxel_data.h"
#include "engine/voxel_direction.h"
#include "world/chunk_manager.h"

void MeshBuilder::buildMeshes(Voxel (& p_voxels) [CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE], glm::ivec3 p_chunk_pos) {
    MeshBuilderContext context {
        p_voxels,
        ChunkManager::get(),
        p_chunk_pos
    };

    CubeTextureData texture_data;
    glm::vec3 global_voxel_pos;
    
    for (int x = 0; x < CHUNK_SIZE; x++) {
        for (int y = 0; y < CHUNK_SIZE; y++) {
            for (int z = 0; z < CHUNK_SIZE; z++) {
                context.current_voxel = p_voxels[x][y][z];

                if (context.current_voxel.id == 0) continue;

                context.current_voxel_pos = {x, y, z};
                context.current_voxel_data = VOXEL_DATAS[context.current_voxel.id-1];

                Mesh& mesh = context.current_voxel_data.transparent ? _transparent_mesh : _opaque_mesh;
                
                texture_data = TEXTURE_COORD[context.current_voxel.id-1];
                global_voxel_pos = p_chunk_pos * CHUNK_SIZE + glm::ivec3(x, y, z);
                
                context.direction = getDirectionVector(Direction::X_PLUS);
                context.neighbor_chunk_voxel_pos = {0, y, z};
                if (checkNeighbor(context)) mesh.add(
                    global_voxel_pos,
                    PLUS_X,
                    context.direction,
                    texture_data.side,
                    context.current_voxel_data.shininess
                );
                context.direction = getDirectionVector(Direction::X_MINUS);
                context.neighbor_chunk_voxel_pos = {CHUNK_SIZE-1, y, z};
                if (checkNeighbor(context)) mesh.add(
                    global_voxel_pos,
                    MINUS_X,
                    context.direction,
                    texture_data.side,
                    context.current_voxel_data.shininess
                );

                context.direction = getDirectionVector(Direction::Y_PLUS);
                context.neighbor_chunk_voxel_pos = {x, 0, z};
                if (checkNeighbor(context)) mesh.add(
                    global_voxel_pos,
                    PLUS_Y,
                    context.direction,
                    texture_data.up,
                    context.current_voxel_data.shininess
                );
                context.direction = getDirectionVector(Direction::Y_MINUS);
                context.neighbor_chunk_voxel_pos = {x, CHUNK_SIZE-1, z};
                if (checkNeighbor(context)) mesh.add(
                    global_voxel_pos,
                    MINUS_Y,
                    context.direction,
                    texture_data.down,
                    context.current_voxel_data.shininess
                );

                context.direction = getDirectionVector(Direction::Z_PLUS);
                context.neighbor_chunk_voxel_pos = {x, y, 0};
                if (checkNeighbor(context)) mesh.add(
                    global_voxel_pos,
                    PLUS_Z,
                    context.direction,
                    texture_data.side,
                    context.current_voxel_data.shininess
                );
                context.direction = getDirectionVector(Direction::Z_MINUS);
                context.neighbor_chunk_voxel_pos = {x, y, CHUNK_SIZE-1};
                if (checkNeighbor(context)) mesh.add(
                    global_voxel_pos,
                    MINUS_Z,
                    context.direction,
                    texture_data.side,
                    context.current_voxel_data.shininess
                );
            }
        }
    }
}

bool MeshBuilder::checkNeighbor(MeshBuilderContext& c) {
    glm::ivec3 neighbor_voxel_pos = c.current_voxel_pos+c.direction;
    if (isInsideChunk(neighbor_voxel_pos)) {
        c.neighbor_voxel = c.voxels[neighbor_voxel_pos.x][neighbor_voxel_pos.y][neighbor_voxel_pos.z];
        c.neighbor_voxel_data = VOXEL_DATAS[c.neighbor_voxel.id-1];

        return c.neighbor_voxel.id == 0 ||
            (! c.current_voxel_data.transparent &&
            c.neighbor_voxel_data.transparent);
    } else {
        c.neighbor_chunk = c.chunk_manager.getChunk(c.chunk_pos + c.direction);

        if (c.neighbor_chunk != nullptr) {
            c.neighbor_voxel = c.neighbor_chunk->getVoxel(c.neighbor_chunk_voxel_pos);
            c.neighbor_voxel_data = VOXEL_DATAS[c.neighbor_voxel.id-1];

            return c.neighbor_voxel.id == 0 ||
                (! c.current_voxel_data.transparent &&
                c.neighbor_voxel_data.transparent);
        }
    }

    return true;
}

bool MeshBuilder::isInsideChunk(glm::ivec3 p_pos) {
    return p_pos.x >= 0 && p_pos.x < CHUNK_SIZE &&
           p_pos.y >= 0 && p_pos.y < CHUNK_SIZE &&
           p_pos.z >= 0 && p_pos.z < CHUNK_SIZE;
}