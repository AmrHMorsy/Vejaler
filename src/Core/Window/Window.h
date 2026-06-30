#pragma once
#include "Data/WindowInfo.h"
#include "../GLFWManager/GLFWManager.h"



class Window {
    
public:
    
    static void Build(WindowInfo& window);
    static void Destroy(WindowInfo& window);
    static void HandleWindowResize(WindowInfo& window);
};
