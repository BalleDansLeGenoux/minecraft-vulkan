#ifndef PROCEDURAL_GENERATOR_HPP
#define PROCEDURAL_GENERATOR_HPP

#include <math.h>

#include "world/chunk.h"
#include "world/perlin_noise_2d.h"


extern int   PROCEDURAL_OCTAVES;
extern float PROCEDURAL_FREQUENCY;
extern float PROCEDURAL_AMPLITUDE;
extern float PROCEDURAL_PERSISTENCE;
extern float PROCEDURAL_MULT_FREQUENCY;


class ProceduralGenerator
{
    private:
        PerlinNoise2D perlin_noise;
    
    public:
        ProceduralGenerator();

        void generateChunk(glm::vec2 pos);
};

#endif