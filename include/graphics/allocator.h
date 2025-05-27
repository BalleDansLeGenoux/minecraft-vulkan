#ifndef VULKAN_ALLOCATOR_H
#define VULKAN_ALLOCATOR_H

#include <cstdint>
#include <vector>
#include <memory>
#include <optional>
#include <glm/glm.hpp>
#include <unordered_map>

#include "graphics/buffer.h"
#include "graphics/vertex.h"
#include "engine/mesh.h"

class Allocator {
public:
    Allocator() {}
    Allocator(int p_flag, uint32_t p_nbBlock, uint32_t p_blockSize, std::reference_wrapper<Buffer> p_staging);
    
    void alloc(void* p_data, uint32_t p_size, uint32_t p_srcOffset, uint32_t p_dstOffset);
    void extractData(void* p_dst, uint32_t p_nbBlock, uint32_t p_offset);

    void cleanup();

    Allocator& operator=(const Allocator& other);

    Buffer& getBuffer() { return _buffer; };
    uint32_t getBlockSize() { return _blockSize; }

private:
    Buffer _buffer;

    uint32_t _blockSize;
    VkDeviceSize _size;

    std::optional<std::reference_wrapper<Buffer>> _staging;
};

#endif