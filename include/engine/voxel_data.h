#ifndef VOXEL_DATA_H
#define VOXEL_DATA_H

#include <vector>

#include "engine/texture_data.h"

struct VoxelData {
    int textures;
    float shininess;
};

extern std::vector<VoxelData> VOXEL_DATAS;

#endif