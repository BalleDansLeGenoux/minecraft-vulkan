#ifndef TEXTURE_COORD_H
#define TEXTURE_COORD_H

#include <vector>
#include <glm/glm.hpp>

struct FaceTextureCoord {
    FaceTextureCoord() {}
    FaceTextureCoord(glm::vec2 point_a, glm::vec2 point_b, glm::vec2 point_c, glm::vec2 point_d) {
        v[0] = point_a;
        v[1] = point_b;
        v[2] = point_c;
        v[3] = point_d;
    }

    glm::vec2 v[4];
};

struct CubeTextureCoord {
    CubeTextureCoord() {}
    CubeTextureCoord(FaceTextureCoord p_up, FaceTextureCoord p_side, FaceTextureCoord p_down)
        : up(p_up), side(p_side), down(p_down) {}

    FaceTextureCoord up;
    FaceTextureCoord side;
    FaceTextureCoord down;
};

extern std::vector<CubeTextureCoord> TEXTURE_COORD;

#endif