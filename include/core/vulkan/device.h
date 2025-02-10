#ifndef VULKAN_DEVICE_H
#define VULKAN_DEVICE_H

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsAndComputeFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsAndComputeFamily.has_value() && presentFamily.has_value();
    }
};

class Device {
public:
    static Device& get() {
        static Device instance; 
        return instance;
    }

    void pickPhysicalDevice();
    void createLogicalDevice();
    void createDepthResources();
    void cleanup();

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice pdevice);
    VkFormat findDepthFormat();

    VkPhysicalDevice& getPhysicalDevice() { return physicalDevice; }
    VkDevice& getDevice() { return device; }

    VkQueue& getGraphicsQueue() { return graphicsQueue; }
    VkQueue& getPresentQueue() { return presentQueue; }
    VkQueue& getComputeQueue() { return computeQueue; }
    
    VkImage& getDepthImage() { return depthImage; }
    VkDeviceMemory& getDepthImageMemory() { return depthImageMemory; }
    VkImageView& getDepthImageView() { return depthImageView; }

    // Device();

private:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkQueue computeQueue;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    bool isDeviceSuitable(VkPhysicalDevice pdevice);
    bool checkDeviceExtensionSupport(VkPhysicalDevice pdevice);
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
};

#endif // VULKAN_DEVICE_H

