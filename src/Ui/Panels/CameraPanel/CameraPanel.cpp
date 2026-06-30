#include "CameraPanel.h"
#include "../../Constants.h"
#include "../../Utilities/Utilities.h"
#include "../../../Scene/Camera/Camera.h"


void Ui::CameraPanel::Render(ContextInfo& context, SceneInfo& scene, float windowWidth, float windowHeight)
{
    if(!showCameraPanel)
        return;
    
    CameraPack& cameraPack = scene.cameraPack;
    
    CameraInfo& camera = scene.cameraPack.cameras[scene.cameraPack.selected];
    
    if(cameraPack.cameras.size() == 0){
        showCameraPanel = false;
        return;
    }
    
    ImGui::SetNextWindowSizeConstraints(ImVec2(450, 450), ImVec2(FLT_MAX, FLT_MAX));

    ImGui::SetNextWindowPos(ImVec2(windowWidth, windowHeight), ImGuiCond_Always, ImVec2(1.0f, 1.0f));

    ImGui::Begin("Camera", &showCameraPanel, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    if(ImGui::BeginTable("Table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)){
        
        ImGui::TableNextRow();
        
        ImGui::TableSetColumnIndex(0);
        Utilities::SetTableBgColor(column0BgColor);
        Utilities::RenderText("Hide All Camera Models");
        ImGui::Separator();
        Utilities::RenderText("Active Camera");
        Utilities::RenderText("Position");
        Utilities::RenderText("Direction");
        Utilities::RenderText("Pitch");
        Utilities::RenderText("Yaw");
        Utilities::RenderText("Roll");
        Utilities::RenderText("Move Speed");
        Utilities::RenderText("Look Around Speed");
        Utilities::RenderText("Fov");
        Utilities::RenderText("Near Clipping Plane");
        Utilities::RenderText("Far Clipping Plane");
        Utilities::RenderText("Camera Collision");
        Utilities::RenderText("Delete Camera");
        
        ImGui::TableSetColumnIndex(1);
        Utilities::SetTableBgColor(column1BgColor);
        if(Utilities::RenderCheckBox("##Hide", cameraPack.hideAllCameraModels, windowWidth/30)){
            for(size_t j = 0; j < cameraPack.cameras.size(); j++){
                if(j == cameraPack.selected)
                    continue;
            }
        }
        ImGui::Separator();
        
        if(Utilities::RenderDropDownList("##SelectedCamera", cameraPack.selected, cameraPack.names, 120.0f)){
            scene.lightCulling.uniformData.nearClippingPlane = cameraPack.cameras[cameraPack.selected].config.nearClippingPlane;
            scene.lightCulling.uniformData.farClippingPlane = cameraPack.cameras[cameraPack.selected].config.farClippingPlane;
            scene.depthPrepass.vsData.cameraViewMatrix = scene.cameraPack.cameras[scene.cameraPack.selected].viewMatrix;
            scene.depthPrepass.vsData.cameraProjectionMatrix = scene.cameraPack.cameras[scene.cameraPack.selected].projectionMatrix;
            scene.normalPass.vsData.cameraViewMatrix = scene.cameraPack.cameras[scene.cameraPack.selected].viewMatrix;
            scene.normalPass.vsData.cameraProjectionMatrix = scene.cameraPack.cameras[scene.cameraPack.selected].projectionMatrix;
            scene.modelPack.vsData.view = scene.cameraPack.cameras[scene.cameraPack.selected].viewMatrix;
            scene.modelPack.vsData.projection = scene.cameraPack.cameras[scene.cameraPack.selected].projectionMatrix;
            scene.modelPack.fsData.cameraPosition = scene.cameraPack.cameras[scene.cameraPack.selected].config.position;
            scene.modelPack.fsData.nearClippingPlane = scene.cameraPack.cameras[scene.cameraPack.selected].config.nearClippingPlane;
            scene.modelPack.fsData.farClippingPlane = scene.cameraPack.cameras[scene.cameraPack.selected].config.farClippingPlane;
        }

        if(Utilities::RenderDragFloat3("##cameraPosition", cameraPack.cameras[cameraPack.selected].config.position.x, ImGui::GetColumnWidth(), 0.1f, 0.0f, 0.0f, "%.2f")){
            /*camera.viewMatrix = ComputeViewMatrix(camera.up, camera.config.direction, camera.config.position);
            camera.projectionViewMatrix = camera.projectionMatrix * camera.viewMatrix;
            camera.viewFrustumPlanes = ComputeViewFrustumPlanes(camera.projectionViewMatrix);*/
        }
        
        if(Utilities::RenderDragFloat3("##cameraDir", cameraPack.cameras[cameraPack.selected].config.direction.x, ImGui::GetColumnWidth(), 0.1f, 0.0f, 0.0f, "%.2f")){
            /*camera.viewMatrix = ComputeViewMatrix(camera.up, camera.config.direction, camera.config.position);
            camera.projectionViewMatrix = camera.projectionMatrix * camera.viewMatrix;
            camera.viewFrustumPlanes = ComputeViewFrustumPlanes(camera.projectionViewMatrix);*/
        }
        
        Utilities::RenderDragFloat("##pitch", cameraPack.cameras[cameraPack.selected].pitch, ImGui::GetColumnWidth(), 0.1f, 0.1f, 10.0f, "%.1f");
        
        Utilities::RenderDragFloat("##yaw", cameraPack.cameras[cameraPack.selected].yaw, ImGui::GetColumnWidth(), 0.1f, 0.1f, 10.0f, "%.1f");
        
        Utilities::RenderDragFloat("##roll", cameraPack.cameras[cameraPack.selected].roll, ImGui::GetColumnWidth(), 0.1f, 0.1f, 10.0f, "%.1f");
        
        Utilities::RenderDragFloat("##Speed", cameraPack.cameras[cameraPack.selected].config.speed, ImGui::GetColumnWidth(), 0.1f, 0.1f, 10.0f, "%.2f");
        
        Utilities::RenderDragFloat("##lookAroundSpeed", cameraPack.cameras[cameraPack.selected].config.lookAroundSpeed, ImGui::GetColumnWidth(), 0.1f, 0.1f, 10.0f, "%.2f");
        
        if(Utilities::RenderSliderFloat("##FOV", cameraPack.cameras[cameraPack.selected].config.fovDegrees, 30.0f, 120.0f, "%.0f°", ImGui::GetColumnWidth())){
            /*camera.fovRadians = glm::radians(camera.config.fovDegrees);
            camera.projectionMatrix = Camera::ComputeProjectionMatrix(camera.resolution, camera.fovRadians, camera.config.nearClippingPlane, camera.config.farClippingPlane);
            camera.inverseProjectionMatrix = glm::inverse(camera.projectionMatrix);
            camera.projectionViewMatrix = camera.projectionMatrix * camera.viewMatrix;
            camera.viewFrustumPlanes = Camera::ComputeViewFrustumPlanes(camera.projectionViewMatrix);*/
        }
        
        if(Utilities::RenderDragFloat("##NearClippingPlane", cameraPack.cameras[cameraPack.selected].config.nearClippingPlane, ImGui::GetColumnWidth(), 0.1f, 0.1f, 10.0f, "%.1f")){
            
            /*camera.projectionMatrix = ComputeProjectionMatrix(camera.resolution, camera.fovRadians, camera.config.nearClippingPlane, camera.config.farClippingPlane);
            camera.inverseProjectionMatrix = glm::inverse(camera.projectionMatrix);
            camera.projectionViewMatrix = camera.projectionMatrix * camera.viewMatrix;
            camera.viewFrustumPlanes = ComputeViewFrustumPlanes(camera.projectionViewMatrix);
            scene.lightCulling.uniformData.nearClippingPlane = cameraPack.cameras[cameraPack.selected].config.nearClippingPlane;*/
        }

        if(Utilities::RenderDragFloat("##FarClippingPlane", cameraPack.cameras[cameraPack.selected].config.farClippingPlane, ImGui::GetColumnWidth(), 0.1f, 0.1f, 10.0f, "%.1f")){
            /*camera.projectionMatrix = ComputeProjectionMatrix(camera.resolution, camera.fovRadians, camera.config.nearClippingPlane, camera.config.farClippingPlane);
            camera.inverseProjectionMatrix = glm::inverse(camera.projectionMatrix);
            camera.projectionViewMatrix = camera.projectionMatrix * camera.viewMatrix;
            camera.viewFrustumPlanes = ComputeViewFrustumPlanes(camera.projectionViewMatrix);
            scene.lightCulling.uniformData.farClippingPlane = cameraPack.cameras[cameraPack.selected].config.farClippingPlane;*/
        }
                
        Utilities::RenderCheckBox("##enableCameraCollision", camera.config.enableCameraCollision, 50.0f);
                
        if(Utilities::RenderButton("Delete Camera", 120.0f))
            Camera::Delete(context, scene);
        
        ImGui::EndTable();
    }
    
    ImGui::End();
}
