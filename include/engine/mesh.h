#ifndef ENGINE_MESH_H
#define ENGINE_MESH_H

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "core/config.h"
#include "world/voxel.h"
#include "graphics/vertex.h"
#include "engine/face_position.h"
#include "engine/texture_data.h"

class Mesh {
public:
    void addQuad(glm::vec3 p_pos, FacePosition p_face_pos, glm::vec3 p_normal, FaceTextureData p_uv, float p_shininess);
    
    void clear();

    std::vector<Vertex>&   getVertex() { return _vertex; }
    std::vector<uint32_t>& getIndex()  { return _index; }
    bool isEmpty() { return (! _vertex.size()); }

private:
    std::vector<Vertex> _vertex;
    std::vector<uint32_t> _index;

    void addIndex();
};

#endif