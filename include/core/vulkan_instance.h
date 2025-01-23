#ifndef VULKAN_INSTANCE_HPP
#define VULKAN_INSTANCE_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

class VulkanApp;

class VulkanInstance {
public:
    void createInstance();
    void setupDebugMessenger();
    void createSurface();
    void cleanup();

    VkInstance getInstance() const { return instance; }
    VkSurfaceKHR getSurface() const { return surface; }

    VulkanInstance(VulkanApp& pvulkanApp)
    : vulkanApp(pvulkanApp) {};

private:
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkSurfaceKHR surface;

    VulkanApp& vulkanApp;

    bool checkValidationLayerSupport();
    std::vector<const char*> getRequiredExtensions();
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
};

#endif // VULKAN_INSTANCE_HPP
