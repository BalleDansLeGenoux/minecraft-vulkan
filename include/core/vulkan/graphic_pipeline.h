#ifndef VULKAN_GRAPHIC_PIPELINE_H
#define VULKAN_GRAPHIC_PIPELINE_H

#include <vulkan/vulkan.h>
#include <vector>
#include <fstream>

class GraphicPipeline {
public:
    static GraphicPipeline& get() {
        static GraphicPipeline instance; 
        return instance;
    }

    void createRenderPass();
    void createDescriptorSetLayout();
    void createGraphicsPipeline();
    void cleanup();
    void cleanupDescriptorSetLayout();

    VkShaderModule createShaderModule(const std::vector<char>& code);
    static std::vector<char> readFile(const std::string& filename);

    VkRenderPass getRenderPass() const { return renderPass; }

    const VkDescriptorSetLayout& getDescriptorSetLayout() { return descriptorSetLayout; }
    const VkPipeline& getGraphicsPipeline() { return graphicsPipeline; }
    const VkPipelineLayout& getPipelineLayout() { return pipelineLayout; }

private:
    VkRenderPass renderPass;

    VkDescriptorSetLayout descriptorSetLayout;
    VkPipelineLayout pipelineLayout;
    VkPipeline graphicsPipeline;
};

#endif // VULKAN_PIPELINE_H