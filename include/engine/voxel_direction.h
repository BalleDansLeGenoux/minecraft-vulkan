#ifndef VOXEL_DIRECTION_H
#define VOXEL_DIRECTION_H

#include <array>

#include <glm/glm.hpp>

enum Direction {
    X_PLUS,
    X_MINUS,
    Y_PLUS,
    Y_MINUS,
    Z_PLUS,
    Z_MINUS,
    COUNT
};

inline glm::ivec3 getDirectionVector(Direction dir) {
    static const std::array<glm::ivec3, static_cast<size_t>(Direction::COUNT)> vectors = {
        glm::ivec3( 1,  0,  0),
        glm::ivec3(-1,  0,  0),
        glm::ivec3( 0,  1,  0),
        glm::ivec3( 0, -1,  0),
        glm::ivec3( 0,  0,  1),
        glm::ivec3( 0,  0, -1)
    };
    return vectors[static_cast<size_t>(dir)];
}

#endif