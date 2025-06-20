#ifndef VULKAN_CONFIG_H
#define VULKAN_CONFIG_H

#include <vector>
#include <cmath>

#include "graphics/swapchain.h"

// #define NDEBUG

#ifdef NDEBUG
const bool enableValidationLayers = false;  // Désactiver les layers en mode release
#else
const bool enableValidationLayers = true;   // Activer les layers en mode debug
#endif

const int RENDER_DISTANCE = 16;
const int CHUNK_SIZE = 16;

const int NB_FACE_CHUNK = std::pow(CHUNK_SIZE/2, 3) * 6;

const int MARGIN_BLOCKS = 1; // coef multiplicateur

#define FRAME_IN_FLIGHT Swapchain::get().getFramesInFlight()

#endif // VULKAN_CONFIG_H
