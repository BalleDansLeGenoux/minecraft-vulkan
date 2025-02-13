#ifndef VULKAN_UNIFORM_BUFFER_H
#define VULKAN_UNIFORM_BUFFER_H

#include <vulkan/vulkan.h>
#include <glm/glm.hpp>

#include <core/vulkan/buffer.h>

struct UniformBufferObject {
    alignas(16) glm::mat4 matrix;
};

class UniformBuffer {
public:
    void createUniformBuffer();
    void updateUniformBuffer(glm::mat4 matrix);
    void cleanup();

    VkBuffer& getBuffer() { return buffer.getBuffer(); }
    VkDeviceMemory& getBufferMemory() { return buffer.getBufferMemory(); }

private:
    Buffer buffer;
    void* uniformBufferMapped;
};

#endif