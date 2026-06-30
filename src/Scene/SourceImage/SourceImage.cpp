#include "SourceImage.h"
#include "../../../Resources/external/include/stb_image/stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../../../Resources/external/include/stb_image/stb_image_resize2.h"
#include <filesystem>



SourceImageInfo SourceImage::LoadLDR(std::string filePath, int res)
{
    std::string extension = std::filesystem::path(filePath).extension().string();
    
    void * image;
    VkDeviceSize imageSize;
    int channels, width, height, desiredNumChannels = 4;
    
    if( (extension == ".png") || (extension == ".jpg")){
        unsigned char * img = stbi_load(filePath.c_str(), &width, &height, &channels, desiredNumChannels);
        imageSize = sizeof(unsigned char);
        image = img;//Resize(img, width, height, res);
    }
    else if(extension == ".dds"){
        filePath.erase(filePath.find(".dds"));
        filePath += ".png";
        unsigned char * img = stbi_load(filePath.c_str(), &width, &height, &channels, 4);
        imageSize = sizeof(unsigned char);
        image = img;
    }
    else
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO LOAD IMAGE::UNSUPPORTED IMAGE EXTENSION\n\033[0m");
    
    imageSize *= width * height * desiredNumChannels;

    uint32_t numMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;

    VkExtent2D resolution = VkExtent2D{
        .width = static_cast<uint32_t>(width),
        .height = static_cast<uint32_t>(height)
    };
    
    return SourceImageInfo{
        .image = image,
        .imageSize = imageSize,
        .resolution = resolution,
        .numMipLevels = numMipLevels,
        .numChannels = desiredNumChannels
    };
}

SourceImageInfo SourceImage::LoadHDR(const char * filePath)
{
    std::string extension = std::filesystem::path(filePath).extension().string();
    
    void * image;
    VkDeviceSize imageSize;
    int channels, width, height, desiredNumChannels = 4;
    
    if(extension == ".hdr"){
        float * img = stbi_loadf(filePath, &width, &height, &channels, desiredNumChannels);
        uint16_t * halfImg = new uint16_t[width*height*desiredNumChannels];
        for(uint32_t i = 0; i < width*height*desiredNumChannels; i++)
            halfImg[i] = FloatToHalf(img[i]);
        image = halfImg;
        imageSize = sizeof(uint16_t);
    }
    else
        throw std::runtime_error("\033[31m\nERROR::FAILURE TO LOAD IMAGE::UNSUPPORTED IMAGE EXTENSION\n\033[0m");
    
    imageSize *= width * height * desiredNumChannels;

    uint32_t numMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;

    VkExtent2D resolution = VkExtent2D{
        .width = static_cast<uint32_t>(width),
        .height = static_cast<uint32_t>(height)
    };
    
    return SourceImageInfo{
        .image = image,
        .imageSize = imageSize,
        .resolution = resolution,
        .numMipLevels = numMipLevels,
        .numChannels = desiredNumChannels
    };
}

unsigned char * SourceImage::Resize(unsigned char * img, int& width, int& height, int resolution)
{
    int targetWidth = width;
    int targetHeight = height;
    if(width > resolution || height > resolution){
        if(width > height) {
            targetWidth = resolution;
            targetHeight = (height * resolution) / width;
        }
        else{
            targetHeight = resolution;
            targetWidth = (width * resolution) / height;
        }
    }

    if((width == targetWidth) && (height == targetHeight))
        return img;
        
    unsigned char * resizedImg = new unsigned char[targetWidth * targetHeight * 4];
    stbir_resize_uint8_srgb(img, width, height, 0, resizedImg, targetWidth, targetHeight, 0, static_cast<stbir_pixel_layout>(4));
    
    width = targetWidth;
    height = targetHeight;
    
    return resizedImg;
}

unsigned char SourceImage::Sample(const SourceImageInfo * src, uint32_t targetX, uint32_t targetY, VkExtent2D dstResoluition, uint32_t channelIndex, unsigned char defaultValue)
{
    if(!src || !src->image)
        return defaultValue;
    
    VkExtent2D srcResoluition = src->resolution;
    
    uint32_t srcX = (targetX * srcResoluition.width) / dstResoluition.width;
    uint32_t srcY = (targetY * srcResoluition.height) / dstResoluition.height;
    size_t index = (srcY * srcResoluition.width + srcX) * src->numChannels + channelIndex;
    
    return static_cast<unsigned char*>(src->image)[index];
}

VkExtent2D SourceImage::GetResolution(const SourceImageInfo* r, const SourceImageInfo* g, const SourceImageInfo* b, const SourceImageInfo* a)
{
    std::vector<uint32_t> widths, heights;
    
    if(r){
        widths.push_back(r->resolution.width);
        heights.push_back(r->resolution.height);
    }
    if(g){
        widths.push_back(g->resolution.width);
        heights.push_back(g->resolution.height);
    }
    if(b){
        widths.push_back(b->resolution.width);
        heights.push_back(b->resolution.height);
    }
    if(a){
        widths.push_back(a->resolution.width);
        heights.push_back(a->resolution.height);
    }
    
    return VkExtent2D{
        .width = *std::max_element(widths.begin(), widths.end()),
        .height = *std::max_element(heights.begin(), heights.end())
    };
}

SourceImageInfo SourceImage::Merge(const SourceImageInfo* r, const SourceImageInfo* g, const SourceImageInfo* b, const SourceImageInfo* a, const glm::ivec4& channelMap)
{
    VkExtent2D res = GetResolution(r, g, b, a);
    
    uint32_t numMipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(res.width, res.height)))) + 1;
    
    VkDeviceSize size = sizeof(unsigned char) * res.width * res.height * 4;
    
    unsigned char * image = new unsigned char[res.width * res.height * 4];
    
    for(uint32_t y = 0; y < res.height; ++y) {
        for(uint32_t x = 0; x < res.width; ++x){
            size_t i = (y * res.width + x);
            image[i*4+0] = Sample(r, x, y, res, channelMap.r, 0);
            image[i*4+1] = Sample(g, x, y, res, channelMap.g, 0);
            image[i*4+2] = Sample(b, x, y, res, channelMap.b, 0);
            image[i*4+3] = Sample(a, x, y, res, channelMap.a, 255);
        }
    }
    
    return SourceImageInfo{
        .image = image,
        .numMipLevels = numMipLevels,
        .resolution = res,
        .imageSize = size,
        .numChannels = 4
    };
}

uint16_t SourceImage::FloatToHalf(float f)
{
    uint32_t x = *(uint32_t*)&f;
    
    uint16_t sign = (x >> 16) & 0x8000;
    uint32_t mant = x & 0x7fffff;
    int exp = ((x >> 23) & 0xff) - 112;
    
    if(exp <= 0){
        if(exp < -10)
            return sign;
        mant = (mant | 0x800000) >> (1 - exp);
        return sign | (mant >> 13);
    }

    if(exp >= 31)
        return sign | 0x7c00;
    
    return sign | (exp << 10) | (mant >> 13);
}

void SourceImage::Free(void * image)
{
    stbi_image_free(image) ;
}
