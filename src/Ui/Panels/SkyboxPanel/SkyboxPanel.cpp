#include "SkyboxPanel.h"
#include "../../Constants.h"
#include "../../Utilities/Utilities.h"




void Ui::SkyboxPanel::Render(ContextInfo& context, SceneInfo& scene, int windowWidth, int windowHeight)
{
    if(!showSkyboxPanel)
        return;
        
    SkyboxPack& skyboxPack = scene.skyboxPack;
    
    if(skyboxPack.skyboxes.size() == 0){
        showSkyboxPanel = false;
        return;
    }
    
    SkyboxInfo& skybox = skyboxPack.skyboxes[skyboxPack.selected];
    
    ImGui::SetNextWindowSizeConstraints(ImVec2(500, 100), ImVec2(FLT_MAX, FLT_MAX));
    
    ImGui::SetNextWindowPos(ImVec2(windowWidth, windowHeight), ImGuiCond_Always, ImVec2(1.0f, 1.0f));

    ImGui::Begin("Skybox", &showSkyboxPanel, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    if(ImGui::BeginTable("Table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)){
        ImGui::TableNextRow();
        
        ImGui::TableSetColumnIndex(0);
        Utilities::SetTableBgColor(column0BgColor);
        Utilities::RenderText("Selected Skybox");
        Utilities::RenderText("HDR Image Resolution");
        Utilities::RenderText("Resolution");
        Utilities::RenderText("HDR Tonemapping");
        Utilities::RenderText("Gamma Correction");
        
        
        ImGui::TableSetColumnIndex(1);
        Utilities::SetTableBgColor(column1BgColor);
        
        if(Utilities::RenderDropDownList("##SelectedSkybox", skyboxPack.selected, skyboxPack.names, 120.0f)){
            //Model::UpdateDescriptorSets(context, scene);
        }
        
        Utilities::RenderNumber(skybox.hdrToCubemapTextureInfo.sourceImage.resolution, "%u x %u");
        
        if(Utilities::RenderDropDownList("##SelectedResolution", skybox.selectedResolution, skybox.resolutionStringsPtr, 150.0f))
            skybox.vsData.mipLevel_isHDRTonemappingEnabled_isGammaCorrectEnabled.r = skybox.selectedResolution;
        
        if(Utilities::RenderCheckBox("##EnableHDRTonemapping", skybox.isHDRTonemappingEnabled, 60.0f))
            skybox.vsData.mipLevel_isHDRTonemappingEnabled_isGammaCorrectEnabled.g = skybox.isHDRTonemappingEnabled;
        
        if(Utilities::RenderCheckBox("##EnableGammaCorrection", skybox.isGammaCorrectEnabled, 60.0f))
            skybox.vsData.mipLevel_isHDRTonemappingEnabled_isGammaCorrectEnabled.b = skybox.isGammaCorrectEnabled;
            
        ImGui::EndTable();
    }
    
    ImGui::End();
}
