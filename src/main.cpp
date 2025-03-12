#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "graphics/app.h"
#include "world/chunk_manager.h"

void run(VulkanApp& app) {
    app.init();

    ChunkManager chunks;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            Chunk& chunk = chunks.addChunk({i, 0, j});

            for (int x = 0; x < 16; x++) {
                for (int y = 0; y < 1; y++) {
                    for (int z = 0; z < 16; z++) {
                        chunk.addVoxel({x, y, z}, 1);
                    }
                }
            }
        }
    }

    // {
    // Chunk& chunk = chunks.addChunk({0, 0, 0});

    // for (int x = 0; x < CHUNK_SIZE; x++) {
    //     for (int z = 0; z < CHUNK_SIZE; z++) {
    //         chunk.addVoxel({x, 0, z}, 1);
    //     }
    // }
    // }

    // {
    // Chunk& chunk = chunks.addChunk({1, 0, 0});

    // for (int x = 0; x < CHUNK_SIZE; x++) {
    //     for (int z = 0; z < CHUNK_SIZE; z++) {
    //         chunk.addVoxel({x, 1, z}, 1);
    //     }
    // }
    // }

    // {
    // Chunk& chunk = chunks.addChunk({1, 0, 1});

    // for (int x = 0; x < CHUNK_SIZE; x++) {
    //     for (int z = 0; z < CHUNK_SIZE; z++) {
    //         chunk.addVoxel({x, 2, z}, 1);
    //     }
    // }
    // }

    // {
    // Chunk& chunk = chunks.addChunk({0, 0, 1});

    // for (int x = 0; x < CHUNK_SIZE; x++) {
    //     for (int z = 0; z < CHUNK_SIZE; z++) {
    //         chunk.addVoxel({x, 3, z}, 1);
    //     }
    // }
    // }

    chunks.update();

    chunks.removeChunk({1, 0, 1});

    Chunk& chunk = chunks.getChunk({0, 0, 0});

    chunk.addVoxel({0, 1, 0}, 1);

    chunk.updateMesh();

    chunk = chunks.addChunk({2, 0, 1});

    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 1; y++) {
            for (int z = 0; z < 16; z++) {
                chunk.addVoxel({x, y, z}, 1);
            }
        }
    }

    chunk.updateMesh();

    while (app.isRun()) {
        app.render();
    }

    app.cleanup();
}

int main(int argc, char const *argv[])
{
    std::vector<BlockUpdate> tmp;

    VulkanApp app({0, 3.0f, 0}, 70, tmp);

    try {
        run(app);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
