#pragma once
#include "../Camera/Camera.h"
#include "../BoundingVolume/BoundingBox/BoundingBox.h"
#include "../BoundingVolume/BoundingSphere/BoundingSphere.h"
#include "../IBL/IBL/IBL.h"
#include "../Model/Model.h"
#include "../Light/Light.h"
#include "../../Ui/Manager/Manager.h"
#include "../Skybox/Skybox.h"
#include "../FrustumCulling/FrustumCulling.h"
#include "../LightCulling/LightCulling.h"
#include "../../Core/Core.h"
#include "../OcclusionCulling/OcclusionCulling.h"
#include "../DepthPyramid/DepthPyramid.h"

#include "Data/SceneInfo.h"
#include "Data/ScenePack.h"



class Scene {
    
public:
    
    static void Add(ScenePack& scenePack, ContextInfo& context);
    static void HandleWindowResize(ContextInfo& context, SceneInfo& scene);
    static void Load(ScenePack& scenePack, ContextInfo& context, WindowInfo& window, nlohmann::json j);
    static void Record(ScenePack& scenePack, ContextInfo& context, uint32_t frame);
    static void Submit(ScenePack& scenePack, ContextInfo& context, uint32_t frame);
    static void SetBackgroundColor(SceneInfo& scene, glm::vec3 color);
};
