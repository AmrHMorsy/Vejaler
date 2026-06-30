#pragma once
#include "../DepthPrepass/DepthPrepass.h"
#include "../FrustumCulling/FrustumCulling.h"
#include "../../Core/Core.h"
#include "../Scene/Data/SceneInfo.h"
#include "../Parser/Parser.h"
#include "../Texture/Texture.h"
#include "../Skybox/Data/SkyboxPack.h"
#include "Data/ModelPack.h"
#include "../BVH/BVH.h"
#include "../Transform/Data/TransformInfo.h"
#include "../SourceImage/SourceImage.h"
#include "../Light/Light.h"
#include "../NormalPass/NormalPass.h"
#include "../SSAO/SSAO.h"
#include "../Loader/Loader.h"
#include "../Transform/Transform.h"
#include "../OcclusionCulling/OcclusionCulling.h"
#include "../Material/Material.h"




class Model{
    
private:
    
    static bool Add(ContextInfo& context, LightPack& lightPack, ModelPack& modelPack, ModelConfig& config);
    
public:
    
    static void Build(ContextInfo& context, SceneInfo& scene);
    static void Load(ContextInfo& context, SceneInfo& scene, const nlohmann::json& modelsJSON);
    static void Render(VkDevice logicalDevice, ModelPack& modelPack, FrustumCullingInfo& frustumCulling, OcclusionCullingInfo& occlusionCulling, VkCommandBuffer& commandBuffer, uint32_t frame);
    static void Delete(ContextInfo& context, SceneInfo& scene, uint32_t modelIndex);
    static void UpdateDescriptorSets(ContextInfo& context, ModelPack& modelPack);
    static void UpdateModelMatricesBuffers(ContextInfo& context, ModelPack& modelPack);
    static void UpdateModelMaxScaleBuffers(ContextInfo& context, ModelPack& modelPack);
    static void UpdateBoundingSphereBuffers(ContextInfo& context, ModelPack& modelPack);    
};
