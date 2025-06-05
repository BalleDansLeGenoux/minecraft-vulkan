#include "engine/voxel_data.h"

std::vector<VoxelData> VOXEL_DATAS = {
    // Grass -> 1
    {
        0,
        0.0f,
        false
    },
    // Dirt -> 2
    {
        1,
        0.0f,
        false
    },
    // Stone -> 3
    {
        2,
        0.0f,
        false
    },
    // Obsidian -> 4
    {
        3,
        200.0f,
        false
    },
    // Iron -> 5
    {
        4,
        200.0f,
        false
    },
    // Glass -> 6
    {
        5,
        0.0f,
        true
    },
    // Leaves -> 7
    {
        6,
        0.0f,
        true
    },
    // Water -> 8
    {
        7,
        200.0f,
        true
    },
    // Sand -> 9
    {
        8,
        0.0f,
        false
    }
};