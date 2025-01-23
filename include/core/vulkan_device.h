#ifndef VULKAN_DEVICE_HPP
#define VULKAN_DEVICE_HPP

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include <optional>
#include <set>

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

class VulkanInstance;
class VulkanDevice;
class VulkanSwapchain;
class VulkanBuffer;

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
    VkQueue getGraphicsQueue() const { return graphicsQueue; }
    VkQueue getPresentQueue() const { return presentQueue; }
    
    VkImage getDepthImage() const { return depthImage; }
    VkDeviceMemory getDepthImageMemory() const { return depthImageMemory; }
    VkImageView getDepthImageView() const { return depthImageView; }

    VulkanDevice(VulkanInstance& pvulkanInstance, VulkanSwapchain& pvulkanSwapchain, VulkanBuffer& pvulkanBuffer)
    : vulkanInstance(pvulkanInstance), vulkanSwapchain(pvulkanSwapchain), vulkanBuffer(pvulkanBuffer) {};

private:
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;

    VulkanInstance& vulkanInstance;
    VulkanSwapchain& vulkanSwapchain;
    VulkanBuffer& vulkanBuffer;

    bool isDeviceSuitable(VkPhysicalDevice pdevice);
    bool checkDeviceExtensionSupport(VkPhysicalDevice pdevice);
    VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
};

#endif // VULKAN_DEVICE_HPP

