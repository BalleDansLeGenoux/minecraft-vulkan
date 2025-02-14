#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H

#include <string>
#include <map>

#include "world/chunk.h"

class ChunkManager {
public:

private:
    std::map<std::string, Chunk> chunks;
};

#endif