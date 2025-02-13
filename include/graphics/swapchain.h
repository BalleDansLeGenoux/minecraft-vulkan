#ifndef VULKAN_SWAPCHAIN_H
#define VULKAN_SWAPCHAIN_H

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <vector>

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class Swapchain {
public:
    static Swapchain& get() {
        static Swapchain instance;
        return instance;
    }

    void createSwapChain(GLFWwindow* window);
    void recreateSwapChain(GLFWwindow* window);
    void createImageViews();
    void cleanup();

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice pdevice);
    VkImageView             createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

    const VkSwapchainKHR&             getSwapChain() { return swapChain; }
    const VkFormat&                   getSwapChainImageFormat() { return swapChainImageFormat; }
    const VkExtent2D&                 getSwapChainExtent() { return swapChainExtent; }
    const std::vector<VkImageView>&   getSwapChainImageViews() { return swapChainImageViews; }
    const std::vector<VkFramebuffer>& getSwapChainFramebuffers() { return swapChainFramebuffers; }
    const VkFramebuffer&              getFramebuffers(uint32_t imageIndex) { return swapChainFramebuffers[imageIndex]; }
    const float                       getAspectRatio() const { return swapChainExtent.width/swapChainExtent.height; }

    void addSwapChainFramebuffers(VkFramebuffer pframeBuffer);

private:
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    std::vector<VkFramebuffer> swapChainFramebuffers;

    VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
    VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
    VkExtent2D chooseSwapExtent(GLFWwindow* window, const VkSurfaceCapabilitiesKHR& capabilities);

};

#endif // VULKAN_SWAPCHAIN_HPP
