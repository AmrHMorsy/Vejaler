#include "LightAddPanel.h"
#include "../../Constants.h"
#include "../../Utilities/Utilities.h"



void Ui::LightAddPanel::Render(ContextInfo& context, SceneInfo& scene, float windowWidth, float windowHeight)
{
    if(!showAddLightWindow)
        return;
    
    LightPack& lightPack = scene.lightPack;
    
    ImGui::SetNextWindowSizeConstraints(ImVec2(500, 300), ImVec2(FLT_MAX, FLT_MAX));

    ImGui::SetNextWindowPos(ImVec2(windowWidth/2.0f, windowHeight/2.0f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

    ImGui::Begin("Add Light", &showAddLightWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    
    if(ImGui::BeginTable("Table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)){
        
        ImGui::TableNextRow();
        
        ImGui::TableSetColumnIndex(0);
        Utilities::SetTableBgColor(column0BgColor);
        
        Utilities::RenderText("Position");
        Utilities::RenderText("Color");
        Utilities::RenderText("Intensity");
        Utilities::RenderText("Speed");
        Utilities::RenderText("Near Clipping Plane");
        Utilities::RenderText("Far Clipping Plane");

        ImGui::TableSetColumnIndex(1);
        Utilities::SetTableBgColor(column1BgColor);
                        
        Utilities::RenderDragFloat3("##lightPosition", lightPack.config.position.x, ImGui::GetColumnWidth(), 0.1f, 0.0f, 0.0f, "%.2f");
        Utilities::RenderColorEdit("##Color", lightPack.config.color, 120.0f);
        Utilities::RenderDragFloat("##Intensity", lightPack.config.intensity, ImGui::GetColumnWidth(), 0.1f, 0.1f, 10.0f, "%.2f");
        Utilities::RenderDragFloat("##speed", lightPack.config.speed, ImGui::GetColumnWidth(), 0.1f, 0.1f, 10.0f, "%.2f");
        Utilities::RenderDragFloat("##NearClippingPlane", lightPack.config.nearClippingPlane, ImGui::GetColumnWidth(), 0.1f, 0.1f, 10.0f, "%.2f");
        Utilities::RenderDragFloat("##FarClippingPlane", lightPack.config.farClippingPlane, ImGui::GetColumnWidth(), 0.1f, 0.1f, 10.0f, "%.2f");

        ImGui::EndTable();
    }

    ImGui::Separator();
    ImGui::SetCursorPosX(500);
    if(ImGui::Button("Add")) {
        //Light::Add(context, scene);
        //Model::UpdateDescriptorSets(context, scene);
        showAddLightWindow = false;
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(0.0f);
    if(ImGui::Button("Cancel"))
        showAddLightWindow = false;
    
    ImGui::End();
}
