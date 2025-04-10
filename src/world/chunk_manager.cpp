#include "world/chunk_manager.h"

#include <iostream>

Chunk* ChunkManager::addChunk(glm::ivec3 pos) {
    std::string id = getStringFromIvec(pos);
    auto chunk = std::make_unique<Chunk>(pos);
    chunk->init();
    chunks[id] = std::move(chunk);
    return chunks[id].get();
}

void ChunkManager::removeChunk(glm::ivec3 pos) {
    std::string id = getStringFromIvec(pos);
    Chunk* chunk = chunks[id].get();

    chunk->cleanup();
    chunks.erase(id);
}

std::string ChunkManager::getStringFromIvec(glm::ivec3 v) {
    return std::to_string(v.x) + "," + std::to_string(v.y) + "," + std::to_string(v.z);
}

void ChunkManager::update() {
    for (auto& [key, chunk] : chunks) {
        chunk->update();
    }
}

void ChunkManager::upload() {
    for (auto& [key, chunk] : chunks) {
        chunk->upload();
    }
}

Chunk* ChunkManager::getChunk(glm::ivec3 pos) {
    std::string id = getStringFromIvec(pos);
    auto it = chunks.find(id);
    if (it == chunks.end() || !it->second) return nullptr;
    return it->second.get();
};