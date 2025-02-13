#ifndef DESCRIPTOR_MANAGER_H
#define DESCRIPTOR_MANAGER_H

#include <vulkan/vulkan.h>
#include <vector>

#include "core/vulkan/renderer.h"

class Descriptor {
public:
    static Descriptor& get() {
        static Descriptor instance;
        return instance;
    }

    void createDescriptorPool();
    void createDescriptorSets();
    void cleanup();

    const std::vector<VkDescriptorSet>& getDescriptorSets() { return descriptorSets; }
    const VkDescriptorSet& getComputeDescriptorSets() { return computeDescriptorSets[Renderer::get().getCurrentFrame()]; }

private:
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
    std::vector<VkDescriptorSet> computeDescriptorSets;
};

#endif // DESCRIPTOR_MANAGER_H
