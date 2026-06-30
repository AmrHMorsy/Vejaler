#pragma once
#include "../../../Includes.h"




namespace Ui{
    struct TextConfig{
        int upIndex = -1;
        int downIndex = -1;
        int waitTime = 3;
        bool bold = false;
        int duration = -1;
        float size = 70.0f;
        bool blink = false;
        bool display = true;
        std::string text = "";
        float blinkSpeed = 1.0f;
        bool progressive = false;
        int variableIndex;
        bool isVariable = false;
        bool fillBackground = false;
        ImVec2 position = ImVec2(0, 0);
        bool hoverUnconditionally = false;
        std::string soundEffectName = "";
        bool silentWhenProgressing = true;
        bool changeColorWhenHovered = false;
        std::string soundEffectFilePath = "";
        ImU32 color = IM_COL32(200, 200, 200, 255);
        ImU32 backgroundColor = IM_COL32(0, 0, 0, 120);
        ImU32 hoverColor = IM_COL32(200, 200, 200, 255);
        ImU32 originalColor = IM_COL32(200, 200, 200, 255);
    }; 
}
