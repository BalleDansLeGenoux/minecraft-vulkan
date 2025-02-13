#ifndef VULKAN_COMPUTE_H 
#define VULKAN_COMPUTE_H

#include <vulkan/vulkan.h>

class ComputePipeline {
public:
    static ComputePipeline& get() {
        static ComputePipeline instance;
        return instance;
    }

    void createComputePipeline();
    void dispatchCompute();
    void createDescriptorSetLayout();
    void cleanup();
    void cleanupDescriptorSetLayout();

    const VkPipeline& getComputePipeline() const { return computePipeline; };
    const VkPipelineLayout& getComputePipelineLayout() const { return pipelineLayout; };
    const VkDescriptorSetLayout& getDescriptorSetLayout() const { return descriptorSetLayout; }

private:
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipeline computePipeline;
    VkPipelineLayout pipelineLayout;

    VkPipelineShaderStageCreateInfo createShaderStage(VkShaderModule shaderModule, VkShaderStageFlagBits stage);
};

#endif