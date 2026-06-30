#pragma once
#include "TextConfig.h"




namespace Ui{
    struct TextInfo{
        int t = 0;
        float minX;
        float maxX;
        float minY;
        float maxY;
        int time = 0;
        TextConfig config;
        ImVec2 textSize;
        size_t characterCount = 0;
    }; 
}
