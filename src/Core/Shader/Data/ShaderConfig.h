#pragma once
#include "../../../Includes.h"





struct ShaderConfig{
    std::vector<std::string> filePaths;
    std::vector<VkShaderStageFlagBits> stageFlags;
};
