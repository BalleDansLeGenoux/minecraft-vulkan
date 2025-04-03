#ifndef ENGINE_MESH_H
#define ENGINE_MESH_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "core/config.h"
#include "world/voxel.h"
#include "graphics/vertex.h"
#include "engine/face_position.h"
#include "engine/texture_coord.h"

class Mesh {
public:
    void cullingMeshing(glm::vec3 chunkPos, Voxel voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]);
    void greedyMeshing(glm::vec3 chunkPos, Voxel voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]);
    
    void clear();

    std::vector<Vertex>&   getVertex() { return vertex; }
    std::vector<uint32_t>& getIndex()  { return index; }
    bool isEmpty() { return (!vertex.size()); }

private:
    std::vector<Vertex> vertex;
    std::vector<uint32_t> index;

    void addQuad(glm::vec3 pos, FacePosition face_pos, glm::vec3 normal, FaceTextureCoord uv);
    void addIndex();
};

#endif