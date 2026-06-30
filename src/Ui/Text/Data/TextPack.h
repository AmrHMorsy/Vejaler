#pragma once
#include "TextInfo.h"




namespace Ui{
    struct TextPack{
        int selected;
        bool usingMouse = false;
        bool usingKeyboard = false;
        std::vector<TextInfo> texts;
        int randomDisplayedTextIndex = -1;
    };
}
