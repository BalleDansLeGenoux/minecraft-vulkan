#include "core/vulkan/compute_pipeline.h"

#include <iostream>
#include <array>
#include <string>

#include "core/vulkan/device.h"
#include "core/vulkan/graphic_pipeline.h"
#include "core/vulkan/descriptor.h"


void ComputePipeline::createDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding voxelBinding{};
    voxelBinding.binding = 0;
    voxelBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    voxelBinding.descriptorCount = 1;
    voxelBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    VkDescriptorSetLayoutBinding updateBinding{};
    updateBinding.binding = 1;
    updateBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    updateBinding.descriptorCount = 1;
    updateBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    VkDescriptorSetLayoutBinding vertexBinding{};
    vertexBinding.binding = 2;
    vertexBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    vertexBinding.descriptorCount = 1;
    vertexBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

    VkDescriptorSetLayoutBinding indexBinding{};
    indexBinding.binding = 3;
    indexBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    indexBinding.descriptorCount = 1;
    indexBinding.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;


    std::array<VkDescriptorSetLayoutBinding, 4> bindings = {voxelBinding, updateBinding, vertexBinding, indexBinding};

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(Device::get().getDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create compute descriptor set layout!");
    }
}

void ComputePipeline::createComputePipeline() {
    VkShaderModule computeShaderModule = GraphicPipeline::get().createShaderModule(GraphicPipeline::get().readFile("res/shaders/comp.spv"));

    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
    pushConstantRange.offset = 0;
    pushConstantRange.size = sizeof(uint32_t);

    VkPipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = &descriptorSetLayout;
    layoutInfo.pushConstantRangeCount = 1;
    layoutInfo.pPushConstantRanges = &pushConstantRange;
    vkCreatePipelineLayout(Device::get().getDevice(), &layoutInfo, nullptr, &pipelineLayout);

    VkComputePipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.stage = createShaderStage(computeShaderModule, VK_SHADER_STAGE_COMPUTE_BIT);
    pipelineInfo.layout = pipelineLayout;
    vkCreateComputePipelines(Device::get().getDevice(), nullptr, 1, &pipelineInfo, nullptr, &computePipeline);

    vkDestroyShaderModule(Device::get().getDevice(), computeShaderModule, nullptr);
}

void ComputePipeline::dispatchCompute() {
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

    if (vkBeginCommandBuffer(Renderer::get().getCurrentCommandBuffers(), &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer for compute!");
    }

    vkCmdBindPipeline(Renderer::get().getCurrentCommandBuffers(), VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline);

    vkCmdBindDescriptorSets(Renderer::get().getCurrentCommandBuffers(), VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, 0, 1, &Descriptor::get().getComputeDescriptorSets(), 0, nullptr);

    vkCmdDispatch(Renderer::get().getCurrentCommandBuffers(), 1, 1, 1);

    if (vkEndCommandBuffer(Renderer::get().getCurrentCommandBuffers()) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer for compute!");
    }

    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &Renderer::get().getCurrentCommandBuffers();

    if (vkQueueSubmit(Device::get().getComputeQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit compute command buffer!");
    }

    vkQueueWaitIdle(Device::get().getComputeQueue());
}

void ComputePipeline::cleanup() {
    vkDestroyPipeline(Device::get().getDevice(), computePipeline, nullptr);
    vkDestroyPipelineLayout(Device::get().getDevice(), pipelineLayout, nullptr);
}

void ComputePipeline::cleanupDescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(Device::get().getDevice(), descriptorSetLayout, nullptr);
}

VkPipelineShaderStageCreateInfo ComputePipeline::createShaderStage(VkShaderModule shaderModule, VkShaderStageFlagBits stage) {
    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = stage;
    shaderStageInfo.module = shaderModule;
    shaderStageInfo.pName = "main";

    return shaderStageInfo;
}