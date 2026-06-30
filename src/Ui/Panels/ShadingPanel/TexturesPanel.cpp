#include "TexturesPanel.h"
#include "../../Constants.h"
#include "../../Utilities/Utilities.h"




void Ui::TexturesPanel::Render(ContextInfo& context, SceneInfo& scene, float panelWidth, ApplicationInfo& application)
{
    ModelPack& modelPack = scene.modelPack;
    
    ModelInfo& model = scene.modelPack.models[scene.modelPack.selected];
    ModelConfig& config = model.config;
        
    if(ImGui::CollapsingHeader("Textures")){
        ImVec2 buttonSize = ImVec2(ImGui::GetColumnWidth()/3.0f, ImGui::GetColumnWidth()/3.0f);

        if(ImGui::BeginTable("Table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)){

            RenderImage(model.material.albedo, application.statePack.states[application.statePack.active].isAcceptingEvents, buttonSize, scene.modelPack.modelSettings[scene.modelPack.selected].albedoMipLevel);
            RenderImage(model.material.normal, application.statePack.states[application.statePack.active].isAcceptingEvents, buttonSize, scene.modelPack.modelSettings[scene.modelPack.selected].albedoMipLevel);
            RenderImage(model.material.roughness, application.statePack.states[application.statePack.active].isAcceptingEvents, buttonSize, scene.modelPack.modelSettings[scene.modelPack.selected].roughnessMipLevel);
            RenderImage(model.material.metallic, application.statePack.states[application.statePack.active].isAcceptingEvents, buttonSize, scene.modelPack.modelSettings[scene.modelPack.selected].metallicMipLevel);
            
            ImGui::EndTable();
        }
    }
    
    if(ImGuiFileDialog::Instance()->Display("LoadImage",ImGuiWindowFlags_NoCollapse, ImVec2(800, 600))){
        application.statePack.states[application.statePack.active].isAcceptingEvents = true;
        if(ImGuiFileDialog::Instance()->IsOk())
            ;//application.requestedAssets.push_back(ImGuiFileDialog::Instance()->GetFilePathName());
        ImGuiFileDialog::Instance()->Close();
    }
}

void Ui::TexturesPanel::RenderImage(TexturePack& texture, bool& isAcceptingEvents, ImVec2 buttonSize, int& selectedResolution)
{
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    Utilities::SetTableBgColor(column0BgColor);
    Utilities::RenderText(texture.config.name.c_str());
    
    ImGui::TableSetColumnIndex(1);
    Utilities::SetTableBgColor(column1BgColor);
    if(Utilities::RenderImageButton(texture.config.uiButtonID.c_str(), texture.uiTextureID, buttonSize)){
        isAcceptingEvents = false;

        IGFD::FileDialogConfig config;
        config.path = ".";
        ImGuiFileDialog::Instance()->OpenDialog("LoadImage", "Choose File", ".png", config);
    }
    Utilities::RenderDropDownList(texture.config.uiResolutionButtonID.c_str(), selectedResolution, texture.resolutionStringsPtr, ImGui::GetColumnWidth());
}
