#ifndef VULKAN_APP_HPP
#define VULKAN_APP_HPP

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <chrono>

#include "core/camera.h"
#include "graphics/config.h"
#include "graphics/buffer.h"
#include "graphics/buffer_manager.h"
#include "graphics/compute_pipeline.h"
#include "graphics/descriptor.h"
#include "graphics/device.h"
#include "graphics/instance.h"
#include "graphics/graphic_pipeline.h"
#include "graphics/renderer.h"
#include "graphics/swapchain.h"
#include "graphics/texture.h"
#include "graphics/vertex.h"
#include "graphics/block_update.h"

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

    VulkanApp(glm::vec3 posCamera, float fov, std::vector<BlockUpdate>& pblockUpdate) :
    generated(0), camera(posCamera, fov, 0), blockUpdate(pblockUpdate) {}

private:
    GLFWwindow* window;

    Camera camera;

    bool framebufferResized = false;
    int generated;

    float deltaTime = 0;
    float lastFrame = 0;

    void computeShader(std::vector<VkSemaphore>& waitSemaphores, std::vector<VkPipelineStageFlags>& waitStages);

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    void updateDeltaTime();

    std::vector<BlockUpdate>& blockUpdate;
};

#endif // VULKAN_APP_HPP