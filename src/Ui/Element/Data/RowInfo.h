#pragma once
#include "../../../Includes.h"


#define IMAGE_BUTTON 0
#define TEXT 1



namespace Ui{
    struct RowInfo{
        int type;
        std::string text;
        ImVec2 buttonSize;
        std::string buttonLabel;
    }; 
}
