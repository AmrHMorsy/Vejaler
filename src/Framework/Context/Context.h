#pragma once
#include "../../Core/Core.h"

#include "Data/ContextInfo.h"




class Context{
    
public:
    
    static void Build(ContextInfo& context, const WindowInfo& window);
    static void HandleWindowResize(ContextInfo& context, const WindowInfo& window);
    static void Destroy(ContextInfo& context);
};
