#pragma once
#include "../../../Includes.h"



struct WindowInfo{
    float xScale;
    float yScale;
    int windowWidth;
    int windowHeight;
    GLFWwindow * handle;
    int frameBufferWidth;
    int frameBufferHeight;
};
