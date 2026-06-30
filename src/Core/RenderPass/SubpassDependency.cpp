#include "SubpassDependency.h"



void SubpassDependency::BuildSubpassDependencies(const std::vector<SubpassDependencyBuildConfiguration>& configurations, std::vector<VkSubpassDependency>& subpassDependencies)
{
    subpassDependencies.resize(configurations.size());
    for(size_t i = 0 ; i < configurations.size(); i++){
        subpassDependencies[i].srcSubpass = configurations[i].srcSubpass;
        subpassDependencies[i].dstSubpass = configurations[i].dstSubpass;
        subpassDependencies[i].srcAccessMask = configurations[i].srcAccessMask;
        subpassDependencies[i].dstAccessMask = configurations[i].dstAccessMask;
        subpassDependencies[i].srcStageMask = configurations[i].srcStageMask;
        subpassDependencies[i].dstStageMask = configurations[i].dstStageMask;
    }
}
