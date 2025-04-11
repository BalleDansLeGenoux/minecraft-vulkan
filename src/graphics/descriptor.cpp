#include "graphics/descriptor.h"

#include <iostream>
#include <array>

#include "core/config.h"
#include "graphics/buffer.h"
#include "graphics/device.h"
#include "graphics/graphic_pipeline.h"
#include "graphics/texture.h"
#include "graphics/compute_pipeline.h"
#include "graphics/buffer_manager.h"
#include "graphics/uniform_buffer.h"

void Descriptor::createDescriptorPool() {
    std::array<VkDescriptorPoolSize, 3> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(FRAME_IN_FLIGHT)*4;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(FRAME_IN_FLIGHT);
    poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[2].descriptorCount = static_cast<uint32_t>(FRAME_IN_FLIGHT);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(FRAME_IN_FLIGHT)*4;

    if (vkCreateDescriptorPool(Device::get().getDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void Descriptor::createDescriptorSets() {
    std::vector<VkDescriptorSetLayout> layouts(FRAME_IN_FLIGHT, GraphicPipeline::get().getDescriptorSetLayout());
    std::vector<VkDescriptorSetLayout> computeLayouts(FRAME_IN_FLIGHT, ComputePipeline::get().getDescriptorSetLayout());

    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(FRAME_IN_FLIGHT);

    allocInfo.pSetLayouts = layouts.data();
    descriptorSets.resize(FRAME_IN_FLIGHT);
    if (vkAllocateDescriptorSets(Device::get().getDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    allocInfo.pSetLayouts = computeLayouts.data();
    computeDescriptorSets.resize(FRAME_IN_FLIGHT);
    if (vkAllocateDescriptorSets(Device::get().getDevice(), &allocInfo, computeDescriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets for compute!");
    }

    for (size_t i = 0; i < FRAME_IN_FLIGHT; i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = BufferManager::get().getUniformBuffer(i).getBuffer();
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = Texture::get().getTextureImageView();
        imageInfo.sampler = Texture::get().getTextureSampler();

        std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[1].dstSet = descriptorSets[i];
        descriptorWrites[1].dstBinding = 1;
        descriptorWrites[1].dstArrayElement = 0;
        descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[1].descriptorCount = 1;
        descriptorWrites[1].pImageInfo = &imageInfo;

        vkUpdateDescriptorSets(Device::get().getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);

        if (descriptorSets[i] == VK_NULL_HANDLE) {
            throw std::runtime_error(std::string("Descriptor set allocation failed for index: ")+std::to_string(i));
        }
    }

    for (size_t i = 0; i < FRAME_IN_FLIGHT; i++) {
        VkDescriptorBufferInfo voxelBufferInfo{};
        voxelBufferInfo.buffer = BufferManager::get().getVoxelBuffer().getBuffer();
        voxelBufferInfo.offset = 0;
        voxelBufferInfo.range = VK_WHOLE_SIZE;

        VkDescriptorBufferInfo updateBufferInfo{};
        updateBufferInfo.buffer = BufferManager::get().getUpdateVoxelBuffer().getBuffer();
        updateBufferInfo.offset = 0;
        updateBufferInfo.range = VK_WHOLE_SIZE;

        VkDescriptorBufferInfo vertexBufferInfo{};
        vertexBufferInfo.buffer = BufferManager::get().getVertexBuffers().getBuffer();
        vertexBufferInfo.offset = 0;
        vertexBufferInfo.range = VK_WHOLE_SIZE;

        VkDescriptorBufferInfo indexBufferInfo{};
        indexBufferInfo.buffer = BufferManager::get().getIndexBuffers().getBuffer();
        indexBufferInfo.offset = 0;
        indexBufferInfo.range = VK_WHOLE_SIZE;

        std::array<VkWriteDescriptorSet, 4> descriptorWritesCompute{};

        descriptorWritesCompute[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWritesCompute[0].dstSet = computeDescriptorSets[i];
        descriptorWritesCompute[0].dstBinding = 0;
        descriptorWritesCompute[0].dstArrayElement = 0;
        descriptorWritesCompute[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptorWritesCompute[0].descriptorCount = 1;
        descriptorWritesCompute[0].pBufferInfo = &voxelBufferInfo;

        descriptorWritesCompute[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWritesCompute[1].dstSet = computeDescriptorSets[i];
        descriptorWritesCompute[1].dstBinding = 1;
        descriptorWritesCompute[1].dstArrayElement = 0;
        descriptorWritesCompute[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptorWritesCompute[1].descriptorCount = 1;
        descriptorWritesCompute[1].pBufferInfo = &updateBufferInfo;

        descriptorWritesCompute[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWritesCompute[2].dstSet = computeDescriptorSets[i];
        descriptorWritesCompute[2].dstBinding = 2;
        descriptorWritesCompute[2].dstArrayElement = 0;
        descriptorWritesCompute[2].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptorWritesCompute[2].descriptorCount = 1;
        descriptorWritesCompute[2].pBufferInfo = &vertexBufferInfo;

        descriptorWritesCompute[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWritesCompute[3].dstSet = computeDescriptorSets[i];
        descriptorWritesCompute[3].dstBinding = 3;
        descriptorWritesCompute[3].dstArrayElement = 0;
        descriptorWritesCompute[3].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptorWritesCompute[3].descriptorCount = 1;
        descriptorWritesCompute[3].pBufferInfo = &indexBufferInfo;

        vkUpdateDescriptorSets(Device::get().getDevice(), static_cast<uint32_t>(descriptorWritesCompute.size()), descriptorWritesCompute.data(), 0, nullptr);

        if (computeDescriptorSets[i] == VK_NULL_HANDLE) {
            throw std::runtime_error(std::string("Compute descriptor set allocation failed for index: ")+std::to_string(i));
        }
    }
}

void Descriptor::cleanup() {
    vkDestroyDescriptorPool(Device::get().getDevice(), descriptorPool, nullptr);
}