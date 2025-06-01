#ifndef VOXEL_H
#define VOXEL_H

#include <cstdint>

struct Voxel {
    uint16_t id;
    bool transparent;

    Voxel() {}

    Voxel(uint16_t pid)
    : id(pid), transparent(false) {}
};

#endif