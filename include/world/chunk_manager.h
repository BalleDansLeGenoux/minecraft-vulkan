#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>

#include "world/chunk.h"

class ChunkManager {
public:
    Chunk& addChunk(glm::ivec3 pos);
    void removeChunk(glm::ivec3 pos);

    void update();

    Chunk& getChunk(glm::ivec3 pos) { return chunks[getStringFromIvec(pos)]; };

private:
    std::unordered_map<std::string, Chunk> chunks;

    std::string getStringFromIvec(glm::ivec3 v);
};

#endif