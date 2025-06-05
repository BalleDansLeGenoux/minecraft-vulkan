#include "engine/sortable_mesh.h"

#include <iostream>
#include <algorithm>

#include "graphics/app.h"

void SortableMesh::add(glm::vec3 p_pos, FacePosition p_face_pos, glm::vec3 p_normal, FaceTextureData p_uv, float p_shininess) {
    glm::vec3 face_pos = p_pos + ((p_face_pos.v[0] + p_face_pos.v[1] + p_face_pos.v[2] + p_face_pos.v[3]) / 4.0f);

    _faces.insert( _faces.end(), {
        {
            {p_pos + p_face_pos.v[0], p_normal, p_uv.v[0], p_shininess},
            {p_pos + p_face_pos.v[1], p_normal, p_uv.v[1], p_shininess},
            {p_pos + p_face_pos.v[2], p_normal, p_uv.v[2], p_shininess},
            {p_pos + p_face_pos.v[3], p_normal, p_uv.v[3], p_shininess}
        },
        0,
        face_pos
    });
}

void SortableMesh::sort() {
    for (FaceData& face : _faces) {
        face.distance = glm::distance(VulkanApp::get().getCamera()->getPosition(), face.center);
    }

    std::sort(_faces.begin(), _faces.end(), [](const FaceData& a, const FaceData& b) {
        return a.distance > b.distance;
    });

    clear();

    for (FaceData face : _faces) {
        Mesh::add(face);
    }
}

bool SortableMesh::isEmpty() {
    return Mesh::isEmpty() && ! _faces.size();
}