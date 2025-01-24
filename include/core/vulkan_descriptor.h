#ifndef DESCRIPTOR_MANAGER_HPP
#define DESCRIPTOR_MANAGER_HPP

#include <vulkan/vulkan.h>
#include <vector>

class VulkanDevice;
class VulkanPipeline;
class VulkanBuffer;
class VulkanTexture;

class VulkanDescriptor {
public:
    void createDescriptorPool();
    void createDescriptorSets();
    void cleanup();

    VulkanDescriptor(VulkanDevice& pvulkanDevice, VulkanPipeline& pvulkanPipeline, VulkanBuffer& pvulkanBuffer, VulkanTexture& pvulkanTexture)
    : vulkanDevice(pvulkanDevice), vulkanPipeline(pvulkanPipeline), vulkanBuffer(pvulkanBuffer), vulkanTexture(pvulkanTexture) {}

    const std::vector<VkDescriptorSet>* getDescriptorSets() const { return &descriptorSets; }

private:
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    VulkanDevice& vulkanDevice;
    VulkanPipeline& vulkanPipeline;
    VulkanBuffer& vulkanBuffer;
    VulkanTexture& vulkanTexture;
};

#endif // DESCRIPTOR_MANAGER_HPP
