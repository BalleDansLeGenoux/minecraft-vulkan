#include "world/procedural_generator.h"

#include "core/config.h"
#include "world/chunk_manager.h"

float PROCEDURAL_OCTAVES = 2;
float PROCEDURAL_FREQUENCY = 0.0054;
float PROCEDURAL_AMPLITUDE = 128;
float PROCEDURAL_PERSISTENCE = 0.1;
float PROCEDURAL_MULT_FREQUENCY = 12;

ProceduralGenerator::ProceduralGenerator() {
    perlin_noise = PerlinNoise2D();
}

void ProceduralGenerator::generateChunk(glm::vec2 pos) {
    float value;
    int height;

    Chunk* chunk;

    for(int x = 0; x < CHUNK_SIZE; x++) {
        for(int z = 0; z < CHUNK_SIZE; z++) {
            height = 0;
            
            for(int i = 0; i < PROCEDURAL_OCTAVES; i++) {
                value = perlin_noise.noise_2d(
                    (pos.x*CHUNK_SIZE+x)*PROCEDURAL_FREQUENCY*std::pow(PROCEDURAL_MULT_FREQUENCY, i),
                    (pos.y*CHUNK_SIZE+z)*PROCEDURAL_FREQUENCY*std::pow(PROCEDURAL_MULT_FREQUENCY, i)
                );
                height += static_cast<int>(value*PROCEDURAL_AMPLITUDE*std::pow(PROCEDURAL_PERSISTENCE, i));
            }

            height += 46;
            
            for (int i = height/CHUNK_SIZE-1; i < height/CHUNK_SIZE; i++) {
                chunk = ChunkManager::get().getChunk({pos.x, i, pos.y});
                if (chunk == nullptr) {
                    chunk = ChunkManager::get().addChunk({pos.x, i, pos.y});
                    chunk->init();
                }
                for (int k = 0; k < CHUNK_SIZE; k++) {
                    chunk->addVoxel({x, k, z}, 1);
                }
            }

            chunk = ChunkManager::get().getChunk({pos.x, height/CHUNK_SIZE, pos.y});
            if (chunk == nullptr) {
                chunk = ChunkManager::get().addChunk({pos.x, height/CHUNK_SIZE, pos.y});
                chunk->init();
            }
            for (int i = 0; i < height % CHUNK_SIZE; i++) {
                chunk->addVoxel({x, i, z}, 1);
            }
        }
    }
}