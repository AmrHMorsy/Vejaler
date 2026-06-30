#pragma once
#include "../../../../includes.h"
#include "../../../Texture/Data/TexturePack.h"
#include "../../Irradiance/Data/IrradianceInfo.h"
#include "../../BRDFIntegrationMap/Data/BRDFIntegrationMapInfo.h"
#include "../../PreFilteredEnvironment/Data/PreFilteredEnvironmentInfo.h"



struct IBLInfo{
    IrradianceInfo irradiance;
    BRDFIntegrationMapInfo brdfIntegrationMap;
    PreFilteredEnvironmentInfo preFilteredEnvironment;
};
