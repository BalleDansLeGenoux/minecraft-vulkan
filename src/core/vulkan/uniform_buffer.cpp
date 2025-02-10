#include "core/vulkan/uniform_buffer.h"

#include <string.h>

void UniformBuffer::createUniformBuffer() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject); 

    buffer.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    vkMapMemory(device, buffer.getBufferMemory(), 0, bufferSize, 0, &uniformBufferMapped);
}

void UniformBuffer::updateUniformBuffer(glm::mat4 matrix) {
    UniformBufferObject ubo{};
    ubo.matrix = matrix;

    memcpy(uniformBufferMapped, &ubo, sizeof(ubo));
}

void UniformBuffer::cleanup() {
    buffer.cleanup();
}
