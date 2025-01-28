#ifndef VULKAN_DEVICE_HPP
#define VULKAN_DEVICE_HPP

#include <vulkan/vulkan.h>
#include <vector>
#include <optional>

class VulkanInstance;
class VulkanDevice;
class VulkanSwapchain;
class VulkanBuffer;
class VulkanCompute;

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsAndComputeFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsAndComputeFamily.has_value() && presentFamily.has_value();
    }
};

class VulkanDevice {
public:
    void pickPhysicalDevice();
    void createLogicalDevice();
    void createDepthResources();
    void cleanup();

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice pdevice);
    VkFormat findDepthFormat();

    VkPhysicalDevice getPhysicalDevice() const { return physicalDevice; }
    VkDevice getDevice() const { return device; }
    VkQueue* getGraphicsQueue() { return &graphicsQueue; }
    VkQueue* getPresentQueue() { return &presentQueue; }
    VkQueue* getComputeQueue() { return &computeQueue; }
    
    VkImage getDepthImage() const { return depthImage; }
    VkDeviceMemory getDepthImageMemory() const { return depthImageMemory; }
    VkImageView getDepthImageView() const { return depthImageView; }

    VulkanDevice(VulkanInstance& pvulkanInstance, VulkanSwapchain& pvulkanSwapchain, VulkanBuffer& pvulkanBuffer, VulkanCompute& pvulkanCompute)
    : vulkanInstance(pvulkanInstance), vulkanSwapchain(pvulkanSwapchain), vulkanBuffer(pvulkanBuffer), vulkanCompute(pvulkanCompute) {};

private:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkQueue computeQueue;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VulkanInstance& vulkanInstance;
    VulkanSwapchain& vulkanSwapchain;
    VulkanBuffer& vulkanBuffer;
    VulkanCompute& vulkanCompute;

    bool isDeviceSuitable(VkPhysicalDevice pdevice);
    bool checkDeviceExtensionSupport(VkPhysicalDevice pdevice);
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
};

#endif // VULKAN_DEVICE_HPP

