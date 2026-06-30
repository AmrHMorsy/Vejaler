#include "ImageView.h"



VkImageView ImageView::Build(ImageViewBuildInfo info)
{
    VkImageViewCreateInfo viewInfo{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = info.image,
        .viewType = info.viewType,
        .format = info.format,
        .subresourceRange.aspectMask = info.aspectMask,
        .subresourceRange.baseMipLevel = info.baseMipLevel,
        .subresourceRange.levelCount = info.levelCount,
        .subresourceRange.baseArrayLayer = info.baseArrayLayer,
        .subresourceRange.layerCount = info.layerCount,
        .components = info.components
    } ;
    
    VkImageView imageView ;
    VkResult result = vkCreateImageView(info.logicalDevice, &viewInfo, nullptr, &imageView) ;
    
    if(result != VK_SUCCESS)
        throw std::runtime_error("\033[31mERROR::FAILURE TO CREATE IMAGE VIEW\033[0m" );

    return imageView ;
}

void ImageView::Destroy(VkDevice logicalDevice, VkImageView imageView)
{
    vkDestroyImageView(logicalDevice, imageView, NULL) ;
}
