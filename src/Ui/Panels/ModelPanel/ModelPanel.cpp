#include "ModelPanel.h"
#include "../../Constants.h"
#include "../../../Scene/Model/Model.h"
#include "../../Utilities/Utilities.h"
#include "../ShadingPanel/ShadingPanel.h"
#include "../TransformPanel/TransformPanel.h"



void Ui::ModelPanel::Render(ContextInfo& context, SceneInfo& scene, ModelPack& modelPack, SkyboxPack& skyboxPack, LightPack& lightPack, int windowWidth, int windowHeight, ApplicationInfo& application)
{
    if(!showModelPanel)
        return;
    
    if(modelPack.models.size() == 0){
        showModelPanel = false;
        return;
    }
    
    ImGui::SetNextWindowSizeConstraints(ImVec2(600, 600), ImVec2(FLT_MAX, FLT_MAX));
    
    ImGui::SetNextWindowPos(ImVec2(windowWidth, windowHeight), ImGuiCond_Always, ImVec2(1.0f, 1.0f));
    
    ImGui::Begin("Model", &showModelPanel, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    
    if(ImGui::BeginTable("Table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)){
        
        ImGui::TableNextRow();
        
        ImGui::TableSetColumnIndex(0);
        Utilities::SetTableBgColor(column0BgColor);
        
        if(!modelPack.applyToAllModels)
            Utilities::RenderText("Selected Model");
        
        Utilities::RenderText("Apply To All Models");
        Utilities::RenderText("Hide");
        Utilities::RenderText("Delete Model");

        ImGui::TableSetColumnIndex(1);
        Utilities::SetTableBgColor(column1BgColor);
        
        int selected = modelPack.selected;
        
        if(!modelPack.applyToAllModels)
            if(Utilities::RenderDropDownList("##SelectedModel", selected, modelPack.namesPtr, windowWidth/10))
                modelPack.selected = selected;
        
        Utilities::RenderCheckBox("##ApplyAllModels", modelPack.applyToAllModels, windowWidth/30);
        Utilities::RenderCheckBox("##Hide", modelPack.models[modelPack.selected].config.hide, windowWidth/30);
        
        if(Utilities::RenderButton("Delete Model", 120.0f))
            Model::Delete(context, scene, modelPack.selected);

        ImGui::EndTable();
    }

    Ui::TransformPanel::Render(context, modelPack, lightPack, skyboxPack);
    Ui::ShadingPanel::Render(context, scene, windowWidth, application);

    ImGui::End();
    
    if(modelPack.applyToAllModels){
        for(size_t i = 0; i < modelPack.models.size(); i++)
            scene.modelPack.modelSettings[i] = scene.modelPack.modelSettings[modelPack.selected];
    }
}
