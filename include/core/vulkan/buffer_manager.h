#ifndef VULKAN_BUFFER_MANAGER_H
#define VULKAN_BUFFER_MANAGER_H

#include <vulkan/vulkan.h>
#include <vector>

#include "core/vulkan/uniform_buffer.h"
#include "core/vulkan/block_update.h"

class Buffer;

class BufferManager {
public:
    static BufferManager& get() {
        static BufferManager instance;
        return instance;
    }

    void createVertexBuffer();
    void createIndexBuffer();
    void createComputeBuffer();
    void cleanupBuffers();

    void createUniformBuffers();
    void updateUniformBuffer(glm::mat4 matrix);
    void cleanupUniformBuffer();

    void updateUpdateVoxelBuffer(std::vector<BlockUpdate>& blockUpdate); // tmp
    void printVertexBuffer(size_t size);
    void printIndexBuffer(size_t size);





    Buffer& getVertexBuffers()     { return vertexBuffer; }
    Buffer& getIndexBuffers()      { return indexBuffer; }
    Buffer& getVoxelBuffer()       { return voxelBuffer; }
    Buffer& getUpdateVoxelBuffer() { return updateVoxelBuffer; }
    UniformBuffer& getUniformBuffer(int i)   { return uniformBuffers[i]; }


private:
    Buffer vertexBuffer;
    Buffer indexBuffer;
    Buffer voxelBuffer;
    Buffer updateVoxelBuffer;
    std::vector<UniformBuffer> uniformBuffers;
};

#endif