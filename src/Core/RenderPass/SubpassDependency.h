#pragma once
#include "../../Includes.h"



struct SubpassDependencyBuildConfiguration{
    uint32_t srcSubpass ;
    uint32_t dstSubpass ;
    uint32_t srcStageMask ;
    uint32_t dstStageMask ;
    uint32_t srcAccessMask ;
    uint32_t dstAccessMask ;
} ;

class RenderPass ;

class SubpassDependency{
    
private:
    
    friend class RenderPass ;

    static void BuildSubpassDependencies(const std::vector<SubpassDependencyBuildConfiguration>& configuration, std::vector<VkSubpassDependency>& subpassDependencies);
} ;
