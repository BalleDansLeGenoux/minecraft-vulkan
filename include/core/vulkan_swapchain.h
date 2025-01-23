#ifndef VULKAN_SWAPCHAIN_HPP
#define VULKAN_SWAPCHAIN_HPP

#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <vulkan/vulkan.h>

class VulkanApp;
class VulkanInstance;
class VulkanDevice;
class VulkanRenderer;

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VulkanSwapchain {
public:
    void createSwapChain();
    void recreateSwapChain();
    void createImageViews();
    void cleanup();

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice pdevice);
    VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    VkSwapchainKHR getSwapChain() const { return swapChain; }
    VkFormat getSwapChainImageFormat() const { return swapChainImageFormat; }
    VkExtent2D getSwapChainExtent() const { return swapChainExtent; }
    std::vector<VkImageView> getSwapChainImageViews() const { return swapChainImageViews; }
    std::vector<VkFramebuffer> getSwapChainFramebuffers() const { return swapChainFramebuffers; }

    void addSwapChainFramebuffers(VkFramebuffer pframeBuffer);

    VulkanSwapchain(VulkanApp& pvulkanApp, VulkanInstance& pvulkanInstance, VulkanDevice& pvulkanDevice, VulkanRenderer& pvulkanRenderer)
    : vulkanApp(pvulkanApp), vulkanInstance(pvulkanInstance), vulkanDevice(pvulkanDevice), vulkanRenderer(pvulkanRenderer) {};

private:
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VulkanApp& vulkanApp;
    VulkanInstance& vulkanInstance;
    VulkanDevice& vulkanDevice;
    VulkanRenderer& vulkanRenderer;

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

};

#endif // VULKAN_SWAPCHAIN_HPP
