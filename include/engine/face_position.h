#ifndef FACE_POSITION_H
#define FACE_POSITION_H

#include <glm/glm.hpp>

struct FacePosition {
    FacePosition() {}
    FacePosition(glm::vec3 point_a, glm::vec3 point_b, glm::vec3 point_c, glm::vec3 point_d)
    {
        v[0] = point_a;
        v[1] = point_b;
        v[2] = point_c;
        v[3] = point_d;
    }

    glm::vec3 v[4];
};

extern FacePosition PLUS_X;
extern FacePosition MINUS_X;
extern FacePosition PLUS_Y;
extern FacePosition MINUS_Y;
extern FacePosition PLUS_Z;
extern FacePosition MINUS_Z;

#endif