#ifndef VULKAN_CONFIG_H
#define VULKAN_CONFIG_H

#include <vector>

// #define NDEBUG

#ifdef NDEBUG
const bool enableValidationLayers = false;  // Désactiver les layers en mode release
#else
const bool enableValidationLayers = true;   // Activer les layers en mode debug
#endif

const int MAX_FRAMES_IN_FLIGHT = 4;

const int SIZE_CHUNK_BUFFER = 256*1024;

const int RENDER_DISTANCE = 16;
const int CHUNK_SIZE = 16;

#endif // VULKAN_CONFIG_H
