#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <thread>

#include "graphics/app.h"
#include "world/chunk_manager.h"
#include "world/procedural_generator.h" 

void procedural() {
    int size = 32;
    ProceduralGenerator p;
    for (int x = -(size/2); x < size/2; x++) {
        for (int y = -(size/2); y < size/2; y++) {
            p.generateChunk({x, y});
        }
    }
    ChunkManager::get().update();
}

void flat() {
    Chunk* chunk;
    for (int cx = 0; cx < 4; cx++) {
        for (int cy = 0; cy < 4; cy++) {
            chunk = ChunkManager::get().addChunk({cx, 0, cy});

            for (int x = 0; x < CHUNK_SIZE; x++) {
                for (int z = 0; z < CHUNK_SIZE; z++) {
                    chunk->addVoxel({x, 0, z}, 1);
                }
            }
        }
    }
    chunk->addVoxel({0, 1, 0}, 1);

    ChunkManager::get().update();
}

void run(VulkanApp& app) {
    app.init();

    // ------------------------ Temps Allocation ------------------------ //

    auto start = std::chrono::high_resolution_clock::now();
    procedural();
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed = end - start;
    
    std::cout << "Temps d'exécution : " << elapsed.count() << " ms" << std::endl;

    // ------------------------------------------------------------------ //

    auto lastTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    double elapsedTime;
    int frameCount = 0;

    while (app.isRun()) {
        currentTime = std::chrono::high_resolution_clock::now();    
        elapsedTime = std::chrono::duration<double, std::milli>(currentTime - lastTime).count();
        frameCount++;
    
        if (elapsedTime >= 1000.0) {
            std::cout << "\rFPS: " << frameCount << " " << std::flush;
            frameCount = 0;
            lastTime = currentTime;
        }
    
        app.render();
    }

    std::cout << std::endl;

    app.cleanup();
}

int main(int argc, char const *argv[])
{
    std::vector<BlockUpdate> tmp;

    VulkanApp app({0, 10.0f, 0}, 70, tmp);

    try {
        run(app);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}