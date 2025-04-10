#ifndef TEXTURE_DATA_H
#define TEXTURE_DATA_H

#include <vector>
#include <glm/glm.hpp>

struct FaceTextureData {
    FaceTextureData() {}
    FaceTextureData(glm::vec2 point_a, glm::vec2 point_b, glm::vec2 point_c, glm::vec2 point_d) {
        v[0] = point_a;
        v[1] = point_b;
        v[2] = point_c;
        v[3] = point_d;
    }

    glm::vec2 v[4];
};

struct CubeTextureData {
    CubeTextureData() {}
    CubeTextureData(FaceTextureData p_up, FaceTextureData p_side, FaceTextureData p_down)
        : up(p_up), side(p_side), down(p_down) {}

    FaceTextureData up;
    FaceTextureData side;
    FaceTextureData down;
};

extern std::vector<CubeTextureData> TEXTURE_COORD;

#endif