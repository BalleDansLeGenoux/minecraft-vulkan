#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <vulkan/vulkan.h>

class VulkanDevice;
class VulkanSwapchain;
class VulkanBuffer;

class VulkanTexture {
public:
    void createTextureImage();
    void createTextureImageView();
    void createTextureSampler();
    void cleanup();

    VulkanTexture(VulkanDevice& pvulkanDevice, VulkanSwapchain& pvulkanSwapchain, VulkanBuffer& pvulkanBuffer)
    : vulkanDevice(pvulkanDevice), vulkanSwapchain(pvulkanSwapchain), vulkanBuffer(pvulkanBuffer) {}

    VkImageView getTextureImageView() const { return textureImageView; }
    VkSampler getTextureSampler() const { return textureSampler; }

private:
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;

    VulkanDevice& vulkanDevice;
    VulkanSwapchain& vulkanSwapchain;
    VulkanBuffer& vulkanBuffer;

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
};

#endif // TEXTURE_MANAGER_HPP
