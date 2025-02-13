#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <vulkan/vulkan.h>


class Texture {
public:
    static Texture& get() {
        static Texture instance;
        return instance;
    }

    void createTextureImage();
    void createTextureImageView();
    void createTextureSampler();
    void cleanup();

    VkImageView& getTextureImageView() { return textureImageView; }
    VkSampler& getTextureSampler() { return textureSampler; }

private:
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;

    void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
    void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
};

#endif // TEXTURE_MANAGER_HPP
