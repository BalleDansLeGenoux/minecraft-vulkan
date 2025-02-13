#ifndef VULKAN_BLOCK_UPDATE_H
#define VULKAN_BLOCK_UPDATE_H

#include <glm/glm.hpp>

struct BlockUpdate {
    glm::ivec4 chunkPos;
    glm::uvec4 blockPos;
    glm::uvec4 blockID;
};

#endif