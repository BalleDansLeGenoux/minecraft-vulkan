#ifndef VULKAN_CONFIG_HPP
#define VULKAN_CONFIG_HPP

#include <vector>

#include "core/vulkan_tools.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;  // Désactiver les layers en mode release
#else
const bool enableValidationLayers = true;   // Activer les layers en mode debug
#endif

const int MAX_FRAMES_IN_FLIGHT = 4;

const int BASE_BUFFER_CAPACITY = 10000000;

const int RENDER_DISTANCE = 16;
const int CHUNK_WIDTH = 16;
const int CHUNK_HEIGHT = 256;

const uint8_t voxel[CHUNK_HEIGHT][CHUNK_WIDTH][CHUNK_WIDTH] = {};

#endif // VULKAN_CONFIG_HPP
