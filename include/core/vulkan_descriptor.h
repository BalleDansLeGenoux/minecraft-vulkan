#ifndef DESCRIPTOR_MANAGER_HPP
#define DESCRIPTOR_MANAGER_HPP

#include <vulkan/vulkan.h>
#include <vector>

#include "core/vulkan_renderer.h"

class VulkanDevice;
class VulkanPipeline;
class VulkanBuffer;
class VulkanTexture;
class VulkanCompute;

class VulkanDescriptor {
public:
    void createDescriptorPool();
    void createDescriptorSets();
    void cleanup();

    VulkanDescriptor(VulkanDevice& pvulkanDevice, VulkanPipeline& pvulkanPipeline, VulkanBuffer& pvulkanBuffer, VulkanTexture& pvulkanTexture, VulkanCompute& pvulkanCompute, VulkanRenderer& pvulkanRenderer)
    : vulkanDevice(pvulkanDevice), vulkanPipeline(pvulkanPipeline), vulkanBuffer(pvulkanBuffer), vulkanTexture(pvulkanTexture), vulkanCompute(pvulkanCompute), vulkanRenderer(pvulkanRenderer) {}

    const std::vector<VkDescriptorSet>* getDescriptorSets() const { return &descriptorSets; }
    VkDescriptorSet* getComputeDescriptorSets() { return &computeDescriptorSets[vulkanRenderer.getCurrentFrame()]; }

private:
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<VkDescriptorSet> computeDescriptorSets;

    VulkanDevice& vulkanDevice;
    VulkanPipeline& vulkanPipeline;
    VulkanBuffer& vulkanBuffer;
    VulkanTexture& vulkanTexture;
    VulkanCompute& vulkanCompute;
    VulkanRenderer& vulkanRenderer;
};

#endif // DESCRIPTOR_MANAGER_HPP
