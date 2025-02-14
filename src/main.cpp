#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "graphics/app.h"

void run(VulkanApp& app) {
    app.init();

    Chunk chunk({0, 0, 0});
    Chunk chunk2({1, 0, 0});
    Chunk chunk3({2, 0, 0});
    chunk.init();
    chunk2.init();
    chunk3.init();

    for (int x = 0; x < CHUNK_SIZE; x++) {
        chunk.addVoxel({x, 0, 0}, 1);
        chunk2.addVoxel({x, 1, 1}, 1);
        chunk3.addVoxel({x, 0, 1}, 1);
    }

    chunk.updateMesh();
    chunk2.updateMesh();
    chunk3.updateMesh();

    while (app.isRun()) {
        app.render();
    }

    app.cleanup();
}

int main(int argc, char const *argv[])
{
    std::vector<BlockUpdate> tmp;

    // BlockUpdate a;

    // for (int x = 0; x < 10; x++) {
    //     for (int z = 0; z < 10; z++) {
    //         a.chunkPos = glm::ivec4(0, 0, 0, 0);
    //         a.blockPos = glm::uvec4(x, 0, z, 0);
    //         a.blockID = glm::uvec4(69, 0, 0, 0);
    //         tmp.push_back(a);
    //     }
    // }



    VulkanApp app({0, 3.0f, 0}, 45, tmp);

    try {
        run(app);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
