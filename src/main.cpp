#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "core/vulkan_app.h"

int main(int argc, char const *argv[])
{
    std::vector<BlockUpdate> tmp;

    BlockUpdate a;

    for (int x = 0; x < 160; x++) {
        for (int z = 0; z < 160; z++) {
            a.chunkPos = glm::ivec4(64, 65, 0, 0);
            a.blockPos = glm::uvec4(x, 0, z, 0);
            a.blockID = glm::uvec4(69, 0, 0, 0);
            tmp.push_back(a);
        }
    }



    VulkanApp app({0.1f, 3.0f, 0.0f}, 45, tmp);

    GLFWwindow* window = app.getWindow();

    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
