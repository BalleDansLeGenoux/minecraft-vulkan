#ifndef VULKAN_PIPELINE_HPP
#define VULKAN_PIPELINE_HPP

#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <vulkan/vulkan.h>

class VulkanDevice;
class VulkanSwapchain;

class VulkanPipeline {
public:
    void createRenderPass();
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    void cleanup();

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

    VkShaderModule createShaderModule(const std::vector<char>& code);
    static std::vector<char> readFile(const std::string& filename);

};

#endif // VULKAN_PIPELINE_HPP