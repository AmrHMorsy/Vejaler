#pragma once
#include "Data/SourceImageInfo.h"





class SourceImage{
    
private:
    
    static unsigned char Sample(const SourceImageInfo * src, uint32_t targetX, uint32_t targetY, VkExtent2D dstResoluition, uint32_t channelIndex, unsigned char defaultValue);
    static VkExtent2D GetResolution(const SourceImageInfo* r, const SourceImageInfo* g, const SourceImageInfo* b, const SourceImageInfo* a);
    static unsigned char * Resize(unsigned char * img, int& width, int& height, int resolution);
    static uint16_t FloatToHalf(float f);

public:

    static SourceImageInfo LoadHDR(const char * filePath);
    static SourceImageInfo LoadLDR(std::string filePath, int res);
    static SourceImageInfo Merge(const SourceImageInfo* r, const SourceImageInfo* g, const SourceImageInfo* b, const SourceImageInfo* a, const glm::ivec4& channelMap);
    static void Free(void * image);
};
