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

void test(ProceduralGenerator& p, glm::vec2 v) {
    p.generateChunk(v);
}

// test multithread mais faut ajouter des mutex, la ça marche pas

// void procedural() {
//     std::vector<std::thread*> v;
//     int size = 32;
//     ProceduralGenerator p;
//     for (int x = -(size/2); x < size/2; x++) {
//         for (int y = -(size/2); y < size/2; y++) {
//             // p.generateChunk({x, y});
//             v.push_back(new std::thread(test, std::ref(p), glm::vec2(x, y)));
//         }
//     }
//     for (auto* t : v) {
//         t->join();
//     }
//     ChunkManager::get().update();
// }

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

    VulkanApp app({0, 150.0f, 0}, 70, tmp);

    try {
        run(app);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}