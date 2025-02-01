#ifndef VULKAN_APP_HPP
#define VULKAN_APP_HPP

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <chrono>

#include "core/camera.h"
#include "core/vulkan_config.h"
#include "core/vulkan_buffer.h"
#include "core/vulkan_compute.h"
#include "core/vulkan_descriptor.h"
#include "core/vulkan_device.h"
#include "core/vulkan_instance.h"
#include "core/vulkan_pipeline.h"
#include "core/vulkan_renderer.h"
#include "core/vulkan_swapchain.h"
#include "core/vulkan_texture.h"
#include "core/vulkan_tools.h"

class VulkanApp {
public:
    void initWindow();
    void initVulkan();
    void mainLoop();
    void run();
    void drawFrame();
    void recordCommandBuffer(uint32_t imageIndex);
    void recordComputeCommandBuffer();
    void cleanup();

    static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

    GLFWwindow* getWindow() const { return window; }
    const float getDeltaTime() const {return deltaTime; }

    VulkanApp(glm::vec3 posCamera, float fov) :
    vulkanBuffer(vulkanDevice, vulkanSwapchain, vulkanPipeline, vulkanRenderer),
    vulkanCompute(vulkanDevice, vulkanPipeline, vulkanDescriptor, vulkanRenderer),
    vulkanDescriptor(vulkanDevice, vulkanPipeline, vulkanBuffer, vulkanTexture, vulkanCompute, vulkanRenderer),
    vulkanDevice(vulkanInstance, vulkanSwapchain, vulkanBuffer, vulkanCompute),
    vulkanInstance(*this),
    vulkanPipeline(vulkanDevice, vulkanSwapchain),
    vulkanRenderer(vulkanDevice, vulkanSwapchain, vulkanPipeline),
    vulkanSwapchain(*this, vulkanInstance, vulkanDevice, vulkanRenderer),
    vulkanTexture(vulkanDevice, vulkanSwapchain, vulkanBuffer),
    generated(0), frameCount(0), fps(0.0f),
    camera(posCamera, fov, 1280/720) {}

private:
    GLFWwindow* window;

    Camera camera;

    bool framebufferResized = false;
    int generated;

    std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
    int frameCount;
    float fps;

    float deltaTime = 0;
    float lastFrame = 0;

    void fpsUpdate();
    void computeShader(std::vector<VkSemaphore>& waitSemaphores, std::vector<VkPipelineStageFlags>& waitStages);

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    void updateDeltaTime();

    VulkanBuffer vulkanBuffer;
    VulkanCompute vulkanCompute;
    VulkanDescriptor vulkanDescriptor;
    VulkanDevice vulkanDevice;
    VulkanInstance vulkanInstance;
    VulkanPipeline vulkanPipeline;
    VulkanRenderer vulkanRenderer;
    VulkanSwapchain vulkanSwapchain;
    VulkanTexture vulkanTexture;
};

#endif // VULKAN_APP_HPP