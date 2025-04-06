#ifndef VULKAN_GRAPHIC_PIPELINE_H
#define VULKAN_GRAPHIC_PIPELINE_H

#include <vulkan/vulkan.h>
#include <vector>
#include <fstream>

#include "graphics/light.h"

struct alignas(16) PushConstantData {
    alignas(16) glm::vec3 viewPos;
    alignas(16) glm::mat4 modelMatrix;
    alignas(16) Light light;
};

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