#include "CameraAddPanel.h"
#include "../../Constants.h"
#include "../../Utilities/Utilities.h"
#include "../../../Scene/Camera/Camera.h"



void Ui::CameraAddPanel::Render(ContextInfo& context, SceneInfo& scene, VkExtent2D swapChainExtent, float windowWidth, float windowHeight)
{
    if(!showAddCameraWindow)
        return;

    CameraPack& cameraPack = scene.cameraPack;
    
    ImGui::SetNextWindowSizeConstraints(ImVec2(450, 300), ImVec2(FLT_MAX, FLT_MAX));

    ImGui::SetNextWindowPos(ImVec2(windowWidth/2.0f, windowHeight/2.0f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));

    ImGui::Begin("Add Camera", &showAddCameraWindow, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    
    if(ImGui::BeginTable("Table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)){
        
        ImGui::TableNextRow();
        
        ImGui::TableSetColumnIndex(0);
        Utilities::SetTableBgColor(column0BgColor);
        
        Utilities::RenderText("Position");
        Utilities::RenderText("Direction");
        Utilities::RenderText("Near Clipping Plane");
        Utilities::RenderText("Far Clipping Plane");
        Utilities::RenderText("speed");
        Utilities::RenderText("LookAround Speed");
        Utilities::RenderText("FOV");

        ImGui::TableSetColumnIndex(1);
        Utilities::SetTableBgColor(column1BgColor);
                            
        Utilities::RenderDragFloat3("##CameraPosition", cameraPack.config.position.x, ImGui::GetColumnWidth(), 0.1f, 0.0f, 0.0f, "%.2f");
        Utilities::RenderDragFloat3("##CameraDirection", cameraPack.config.direction.x, ImGui::GetColumnWidth(), 0.1f, 0.0f, 0.0f, "%.2f");
        Utilities::RenderDragFloat("##NearClippingPlane", cameraPack.config.nearClippingPlane, ImGui::GetColumnWidth(), 0.1f, 0.1f, 10.0f, "%.2f");
        Utilities::RenderDragFloat("##FarClippingPlane", cameraPack.config.farClippingPlane, ImGui::GetColumnWidth(), 0.1f, 0.1f, 10.0f, "%.2f");
        Utilities::RenderDragFloat("##speed", cameraPack.config.speed, ImGui::GetColumnWidth(), 0.1f, 0.1f, 10.0f, "%.2f");
        Utilities::RenderDragFloat("##lookaroundSpeed", cameraPack.config.lookAroundSpeed, ImGui::GetColumnWidth(), 0.1f, 0.1f, 10.0f, "%.2f");
        Utilities::RenderSliderFloat("##FOV", cameraPack.config.fovDegrees, 30.0f, 120.0f, "%.0f°", ImGui::GetColumnWidth());

        ImGui::EndTable();
    }

    ImGui::Separator();
    ImGui::SetCursorPosX(450);
    if(ImGui::Button("Add")) {
        Camera::Add(context, scene, swapChainExtent);
        showAddCameraWindow = false;
    }
    ImGui::SameLine();
    ImGui::SetCursorPosX(0.0f);
    if(ImGui::Button("Cancel"))
        showAddCameraWindow = false;
    
    ImGui::End();
}
