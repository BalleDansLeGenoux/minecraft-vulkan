#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <memory>

#include "world/chunk.h"

class ChunkManager {
public:
    Chunk& addChunk(glm::ivec3 pos);
    void removeChunk(glm::ivec3 pos);

    void update();

    static std::string getStringFromIvec(glm::ivec3 v);

    Chunk& getChunk(glm::ivec3 pos) { return *chunks[getStringFromIvec(pos)]; };

private:
    std::unordered_map<std::string, std::unique_ptr<Chunk>> chunks;
};

#endif