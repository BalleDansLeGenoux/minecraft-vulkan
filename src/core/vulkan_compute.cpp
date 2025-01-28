#include "core/vulkan_compute.h"

#include <iostream>
#include <array>
#include <string>

#include "core/vulkan_device.h"
#include "core/vulkan_pipeline.h"
#include "core/vulkan_descriptor.h"


void VulkanCompute::createDescriptorSetLayout() {
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

    if (vkCreateDescriptorSetLayout(vulkanDevice.getDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create compute descriptor set layout!");
    }
}

void VulkanCompute::createComputePipeline() {
    VkShaderModule computeShaderModule = vulkanPipeline.createShaderModule(vulkanPipeline.readFile("res/shaders/comp.spv"));

    VkPipelineLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = &descriptorSetLayout;
    vkCreatePipelineLayout(vulkanDevice.getDevice(), &layoutInfo, nullptr, &pipelineLayout);

    VkComputePipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.stage = createShaderStage(computeShaderModule, VK_SHADER_STAGE_COMPUTE_BIT);
    pipelineInfo.layout = pipelineLayout;
    vkCreateComputePipelines(vulkanDevice.getDevice(), nullptr, 1, &pipelineInfo, nullptr, &computePipeline);

    vkDestroyShaderModule(vulkanDevice.getDevice(), computeShaderModule, nullptr);
}

void VulkanCompute::dispatchCompute() {// Étape 1 : Commencer l'enregistrement du command buffer
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;

    if (vkBeginCommandBuffer(*vulkanRenderer.getCurrentCommandBuffers(), &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer for compute!");
    }

    // Étape 2 : Lier le pipeline de compute
    vkCmdBindPipeline(*vulkanRenderer.getCurrentCommandBuffers(), VK_PIPELINE_BIND_POINT_COMPUTE, computePipeline);

    // Étape 3 : Lier les descripteurs nécessaires (buffers, images, etc.)
    vkCmdBindDescriptorSets(*vulkanRenderer.getCurrentCommandBuffers(), VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, 0, 1, vulkanDescriptor.getComputeDescriptorSets(), 0, nullptr);

    // Étape 4 : Dispatcher le compute shader (définir le nombre de workgroups à exécuter)
    vkCmdDispatch(*vulkanRenderer.getCurrentCommandBuffers(), 1, 1, 1);  // Dispatche 1 workgroup dans chaque dimension

    // Étape 5 : Terminer l'enregistrement du command buffer
    if (vkEndCommandBuffer(*vulkanRenderer.getCurrentCommandBuffers()) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer for compute!");
    }

    // Étape 6 : Soumettre le command buffer à la compute queue
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = vulkanRenderer.getCurrentCommandBuffers();

    if (vkQueueSubmit(*vulkanDevice.getComputeQueue(), 1, &submitInfo, VK_NULL_HANDLE) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit compute command buffer!");
    }

    // Étape 7 : Attendre la fin de l'exécution du compute shader
    vkQueueWaitIdle(*vulkanDevice.getComputeQueue());
}

void VulkanCompute::cleanup() {
    vkDestroyPipeline(vulkanDevice.getDevice(), computePipeline, nullptr);
    vkDestroyPipelineLayout(vulkanDevice.getDevice(), pipelineLayout, nullptr);
}

void VulkanCompute::cleanupDescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(vulkanDevice.getDevice(), descriptorSetLayout, nullptr);
}

VkPipelineShaderStageCreateInfo VulkanCompute::createShaderStage(VkShaderModule shaderModule, VkShaderStageFlagBits stage) {
    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = stage;
    shaderStageInfo.module = shaderModule;
    shaderStageInfo.pName = "main";

    return shaderStageInfo;
}