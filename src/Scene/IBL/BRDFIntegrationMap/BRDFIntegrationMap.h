#pragma once
#include "../../../includes.h"
#include "../../Texture/Texture.h"
#include "../../../Core/Core.h"
#include "Data/BRDFIntegrationMapInfo.h"




class BRDFIntegrationMap{
    
public:
    
    static void Build(ContextInfo& context, BRDFIntegrationMapInfo& brdfIntegrationMap, VkExtent2D resolution);
    static void Record(ContextInfo& context, BRDFIntegrationMapInfo& brdfIntegrationMap);
    static void Submit(ContextInfo& context, BRDFIntegrationMapInfo& brdfIntegrationMap);
};
