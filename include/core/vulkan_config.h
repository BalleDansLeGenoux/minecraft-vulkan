#ifndef VULKAN_CONFIG_HPP
#define VULKAN_CONFIG_HPP

#include <vector>

#include "core/vulkan_tools.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;  // Désactiver les layers en mode release
#else
const bool enableValidationLayers = true;   // Activer les layers en mode debug
#endif

const int MAX_FRAMES_IN_FLIGHT = 2;

const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.5f}},
    {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.5f, 0.5f}},
    {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.5f, 1.0f}},
    {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

    {{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.5f, 0.0f}},
    {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.5f, 0.5f}},
    {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.5f}},

    {{-0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.5f, 0.0f}},
    {{-0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.5f, 0.5f}},
    {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.5f}},

    {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.5f, 0.0f}},
    {{0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.5f, 0.5f}},
    {{0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.5f}},

    {{0.5f, -0.5f, 0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}, {0.5f, 0.0f}},
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {0.5f, 0.5f}},
    {{0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.5f}},
};

const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0,
    4, 5, 6, 6, 7, 4,
    8, 9, 10, 10, 11, 8,
    12, 13, 14, 14, 15, 12,
    16, 17, 18, 18, 19, 16
};

#endif // VULKAN_CONFIG_HPP
