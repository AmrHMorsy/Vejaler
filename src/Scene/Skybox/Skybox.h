#include "../IBL/IBL/IBL.h"
#include "../Model/Model.h"
#include "Data/SkyboxPack.h"
#include "../Texture/Texture.h"
#include "../../Core/Core.h"
#include "../Scene/Data/ScenePack.h"
#include "../SourceImage/SourceImage.h"
#include "Data/SkyboxVertexShaderUniformVariables.h"
#include "Data/HDRToCubemapVertexShaderUniformVariables.h"



class Skybox {
    
private:

    static void Record(ContextInfo& context, SkyboxPack& skyboxPack);
    static void RecordFace(ContextInfo& context, SkyboxPack& skyboxPack, uint32_t mip, uint32_t i);
    static void Submit(ContextInfo& context, SkyboxPack& skyboxPack);
    
public:
        
    static nlohmann::json Save(SkyboxPack& skyboxPack);
    static void Build(ContextInfo& context, SceneInfo& scene);
    static void Add(ContextInfo& context, SceneInfo& scene, const std::string& filePath);
    static void Load(ContextInfo& context, SceneInfo& scene, const nlohmann::json& modelsJSON);
    static void Render(SceneInfo& scene, uint32_t frame);
};
