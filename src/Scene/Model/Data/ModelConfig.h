#pragma once
#include "../../BVH/Data/BVHInfo.h"
#include "../../DepthPrepass/Data/DepthPrepassInfo.h"
#include "../../FrustumCulling/Data/FrustumCullingInfo.h"
#include "../../BoundingVolume/BoundingBox/Data/BoundingBoxInfo.h"
#include "../../Transform/Data/TransformInfo.h"
#include "../../Parser/Data/Mesh.h"
#include "../../BoundingVolume/BoundingBox/Data/BoundingBoxInfo.h"
#include "../../Texture/Data/TexturePack.h"
#include "../../Texture/Data/TextureConfig.h"
#include "../../../Includes.h"
#include "../../../Core/Buffer/Data/UniformBufferPack.h"
#include "../../../Core/Core.h"
#include "../../Texture/Data/TexturePack.h"
#include "ModelConstants.h"
#include "ModelSettings.h"
#include "../../Material/Data/MaterialConfig.h"




struct ModelConfig{
    std::string name;
    bool hide = false;
    int selectedTexture;
    std::string filePath;
    bool castShadow = true;
    ModelSettings modelSettings;
    MaterialConfig materialConfig;
    int selectedShadingModel = PBR;
    TransformConfig transformConfig;
    int boundingBoxType = EXCLUSIVE;
    bool frustumCullingEnable = true;
};
