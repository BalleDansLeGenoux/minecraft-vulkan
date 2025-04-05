#include "graphics/allocator.h"

#include <iostream>
#include <string.h>

#include "graphics/device.h"
#include "graphics/buffer_manager.h"

Allocator::Allocator(int p_usage, uint32_t p_nbBlock, uint32_t p_blockSize, std::reference_wrapper<Buffer> p_staging)
: _blockSize(p_blockSize), _staging(std::make_optional(p_staging)), _size(p_nbBlock*p_blockSize)
{
    _buffer.createBuffer(_size, p_usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    static int tmp = 0;
}

void Allocator::alloc(void* p_data, uint32_t p_nbBlock, uint32_t p_offset) {
    if (p_offset > _size) throw std::runtime_error("Dépassement de mémoire GPU");

    uint32_t size = p_nbBlock * _blockSize;

    void* data;
    
    vkMapMemory(Device::get().getDevice(), _staging.value().get().getBufferMemory(), 0, size, 0, &data);
    memcpy(data, p_data, size);
    vkUnmapMemory(Device::get().getDevice(), _staging.value().get().getBufferMemory());

    BufferManager::copyBuffer(_staging.value().get(), _buffer, size, 0, p_offset * _blockSize);
}

void Allocator::extractData(void* p_dst, uint32_t p_nbBlock, uint32_t p_offset) {
    uint32_t size = p_nbBlock * _blockSize;

    BufferManager::copyBuffer(_buffer, _staging.value().get(), size, p_offset * _blockSize, 0);

    void* data;
    vkMapMemory(Device::get().getDevice(), _staging.value().get().getBufferMemory(), 0, size, 0, &data);
    memcpy(p_dst, data, size);
    vkUnmapMemory(Device::get().getDevice(), _staging.value().get().getBufferMemory());
}

void Allocator::cleanup() {
    _buffer.cleanup();
}

Allocator& Allocator::operator=(const Allocator& other) {
    this->_blockSize = other._blockSize;
    this->_buffer = other._buffer;
    this->_size = other._size;

    if (other._staging.has_value()) {
        this->_staging = std::make_optional(other._staging.value());
    } else {
        this->_staging.reset();
    }

    return *this;
}