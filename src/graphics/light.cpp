#include "graphics/light.h"

DirectionalLight sun_light {
    glm::vec3(1, -1, 1),
    glm::vec3(0.1),
    glm::vec3(0.4),
    glm::vec3(1.0f, 1.0f, 0.0f)
};

DirectionalLight moon_light {
    glm::vec3(1, -1, 1),
    glm::vec3(0.01),
    glm::vec3(0.1),
    glm::vec3(0.5, 0.5, 0.6)
};