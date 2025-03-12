#include "world/chunk_manager.h"

#include <iostream>

#include "world/chunk_manager.h"

Chunk& ChunkManager::addChunk(glm::ivec3 pos) {
    std::string id = getStringFromIvec(pos);
    auto chunk = std::make_unique<Chunk>(pos);
    chunk->init();
    chunks[id] = std::move(chunk);
    return *chunks[id];
}

void ChunkManager::removeChunk(glm::ivec3 pos) {
    std::string id = getStringFromIvec(pos);
    Chunk& chunk = *chunks[id];

    chunk.cleanup();
    chunks.erase(id);
}

std::string ChunkManager::getStringFromIvec(glm::ivec3 v) {
    return std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z);
}

void ChunkManager::update() {
    for (auto& [key, chunk] : chunks) {
        chunk->updateMesh();
    }
}