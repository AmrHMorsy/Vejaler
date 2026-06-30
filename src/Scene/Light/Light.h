#pragma once
#include "../Model/Model.h"
#include "Data/LightPack.h"
#include "Data/LightInfo.h"
#include "../Shadow/Shadow.h"
#include "Data/LightConfig.h"
#include "../Scene/Data/SceneInfo.h"
#include "../../Core/Core.h"
#include "../PoissonDisk/PoissonDisk.h"
#include "../LightCulling/LightCulling.h"



class Light {
    
private:
    
    static void ComputeViewMatrices(LightInfo& light);
    static void ComputeBoundingSphere(LightInfo& light);
    static void ComputeProjectionMatrix(LightInfo& light);
    static void Generate(LightPack& lightPack, ModelPack& modelPack);
    static void GenerateSoftShadowSampleDirections(LightPack& lightPack);
    
public:
    
    static void Load(ContextInfo& context, LightPack& lightPack, ModelPack& modelPack, CameraInfo& camera);
    static void Build(ContextInfo& context, LightPack& lightPack, DepthPrepassInfo& depthPrepass, LightCullingInfo& lightCulling, ModelPack& modelPack, CameraInfo& camera);
};
