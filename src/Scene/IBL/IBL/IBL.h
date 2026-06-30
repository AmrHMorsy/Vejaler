#pragma once
#include "../Irradiance/Irradiance.h"
#include "../BRDFIntegrationMap/BRDFIntegrationMap.h"
#include "../PreFilteredEnvironment/PreFilteredEnvironment.h"
#include "Data/IBLInfo.h"
#include "../../Skybox/Data/SkyboxInfo.h"


class IBL{

public:
        
    static void Build(ContextInfo& context, SkyboxInfo& skybox);
    static void Record(ContextInfo& context, SkyboxInfo& skybox);
    static void Submit(ContextInfo& context, SkyboxInfo& skybox);
};
