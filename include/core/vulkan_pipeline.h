#ifndef VULKAN_PIPELINE_HPP
#define VULKAN_PIPELINE_HPP

#include <vulkan/vulkan.h>
#include <vector>
#include <fstream>

class VulkanDevice;
class VulkanSwapchain;

class VulkanPipeline {
public:
    void createRenderPass();
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    void cleanup();
    void cleanupDescriptorSetLayout();

    VkShaderModule createShaderModule(const std::vector<char>& code);
    static std::vector<char> readFile(const std::string& filename);

    VkRenderPass getRenderPass() const { return renderPass; }

    VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }
    const VkPipeline* getGraphicsPipeline() const { return &graphicsPipeline; }
    const VkPipelineLayout* getPipelineLayout() const { return &pipelineLayout; }

    VulkanPipeline(VulkanDevice& pvulkanDevice, VulkanSwapchain& pvulkanSwapchain)
    : vulkanDevice(pvulkanDevice), vulkanSwapchain(pvulkanSwapchain) {}

private:
    VkRenderPass renderPass;

    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;

    VulkanDevice& vulkanDevice;
    VulkanSwapchain& vulkanSwapchain;

};

#endif // VULKAN_PIPELINE_HPP