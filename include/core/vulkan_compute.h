#ifndef VULKAN_COMPUTE_H 
#define VULKAN_COMPUTE_H

#include <vulkan/vulkan.h>

class VulkanDevice;
class VulkanPipeline;
class VulkanDescriptor;
class VulkanRenderer;

class VulkanCompute {
public:
    void createComputePipeline();
    void dispatchCompute();
    void createDescriptorSetLayout();
    void cleanup();
    void cleanupDescriptorSetLayout();

    VkPipeline* getComputePipeline() { return &computePipeline; };
    VkPipelineLayout* getComputePipelineLayout() { return &pipelineLayout; };
    const VkDescriptorSetLayout& getDescriptorSetLayout() const { return descriptorSetLayout; }

    VulkanCompute(VulkanDevice& pvulkanDevice, VulkanPipeline& pvulkanPipeline, VulkanDescriptor& pvulkanDescriptor, VulkanRenderer& pvulkanRenderer)
    : vulkanDevice(pvulkanDevice), vulkanPipeline(pvulkanPipeline), vulkanDescriptor(pvulkanDescriptor), vulkanRenderer(pvulkanRenderer) {}

private:
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipeline computePipeline;
    VkPipelineLayout pipelineLayout;

    VkPipelineShaderStageCreateInfo createShaderStage(VkShaderModule shaderModule, VkShaderStageFlagBits stage);

    VulkanDevice& vulkanDevice;
    VulkanPipeline& vulkanPipeline;
    VulkanDescriptor& vulkanDescriptor;
    VulkanRenderer& vulkanRenderer;
};

#endif