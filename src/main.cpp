#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

#include "graphics/app.h"
#include "world/chunk_manager.h"
#include "world/procedural_generator.h"

void procedural() {
    int size = 16;
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

    auto start = std::chrono::high_resolution_clock::now();  // Début
    // allocate(chunks);
    procedural();
    auto end = std::chrono::high_resolution_clock::now();    // Fin

    // Calcul du temps en millisecondes
    std::chrono::duration<double, std::milli> elapsed = end - start;
    
    std::cout << "Temps d'exécution : " << elapsed.count() << " ms" << std::endl;

    // ------------------------------------------------------------------ //

    while (app.isRun()) {
        app.render();
    }

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