#ifndef ENGINE_MESH_H
#define ENGINE_MESH_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "core/config.h"
#include "world/voxel.h"
#include "graphics/vertex.h"

class Mesh {
public:
    void cullingMeshing(glm::vec3 chunkPos, Voxel voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]);
    void greedyMeshing(glm::vec3 chunkPos, Voxel voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]);
    
    void clear();

    std::vector<Vertex>& getVertex() { return vertex; }
    std::vector<uint32_t>& getIndex() { return index; }

private:
    std::vector<Vertex> vertex;
    std::vector<uint32_t> index;

    void addQuad(glm::vec3 pos, glm::vec3 normal, glm::vec2 uv);
};

#endif