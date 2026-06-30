#pragma once
#include "../../Includes.h"
#include "../../Core/Core.h"
#include "Data/LightCullingInfo.h"
#include "../Scene/Data/SceneInfo.h"
#include "../Light/Data/LightInfo.h"
#include "../DepthPrepass/Data/DepthPrepassInfo.h"



class LightCulling{

public:
    
    static void Load(ContextInfo& context, LightPack& lightPack, ModelPack& modelPack, CameraInfo& camera);
    static void UpdateLightBoundingSpheres(LightPack& lightPack, CameraInfo& camera);
    static void Build(ContextInfo& context, LightCullingInfo& lightCulling, DepthPrepassInfo& depthPrepass, ModelPack& modelPack, LightPack& lightPack, CameraInfo& camera);
    static void Record(ContextInfo& context, LightPack& lightPack, CameraInfo& camera, uint32_t frame);
    static void Submit(ContextInfo& context, LightCullingInfo& lightCulling, DepthPrepassInfo& depthPrepass, uint32_t frame);
    static void HandleWindowResize(ContextInfo& context, LightCullingInfo& lightCulling, ModelPack& modelPack, CameraInfo& camera);
    static std::vector<glm::vec4> ExtractTilesFrustumPlanes(glm::uvec2 numTiles2D, glm::mat4 cameraProjectionMatrix);
};
