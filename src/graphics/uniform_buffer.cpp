#include "graphics/uniform_buffer.h"

#include <string.h>
#include <stdexcept>

#include "graphics/device.h"

void UniformBuffer::createUniformBuffer() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject); 

    buffer.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    VkResult result = vkMapMemory(Device::get().getDevice(), buffer.getBufferMemory(), 0, bufferSize, 0, &uniformBufferMapped);
    
    if (result != VK_SUCCESS) {
        throw std::runtime_error("Failed to map uniform buffer memory!");
    }
}

void UniformBuffer::updateUniformBuffer(glm::mat4 matrix) {
    UniformBufferObject ubo{};
    ubo.matrix = matrix;

    memcpy(uniformBufferMapped, &ubo, sizeof(ubo));
}

void UniformBuffer::cleanup() {
    buffer.cleanup();
}
