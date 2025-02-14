#include "world/chunk_manager.h"

Chunk& ChunkManager::addChunk(glm::ivec3 pos) {
    std::string id = getStringFromIvec(pos);
    chunks[id] = Chunk(pos);
    Chunk& chunk = chunks[id];
    chunk.init();
    return chunk;
}

std::string ChunkManager::getStringFromIvec(glm::ivec3 v) {
    return std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z);
}

void ChunkManager::update() {
    for (auto chunk : chunks) {
        chunk.second.updateMesh();
    }
}