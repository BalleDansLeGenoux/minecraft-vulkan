#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <memory>

#include "world/chunk.h"

class ChunkManager {
public:
    static ChunkManager& get() {
        static ChunkManager instance;
        return instance;
    }

    Chunk* addChunk(glm::ivec3 pos);
    void removeChunk(glm::ivec3 pos);

    void update();
    void upload();

    static std::string getStringFromIvec(glm::ivec3 v);

    Chunk* getChunk(glm::ivec3 pos);
    int getNumberOfChunk() { return chunks.size(); }

private:
    std::unordered_map<std::string, std::unique_ptr<Chunk>> chunks;
};

#endif