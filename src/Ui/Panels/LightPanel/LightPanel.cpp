#include "LightPanel.h"
#include "../../Constants.h"
#include "../../Utilities/Utilities.h"




void Ui::LightPanel::Render(ContextInfo& context, SceneInfo& scene, int windowWidth, int windowHeight)
{
    if(!showLightPanel)
        return;
    
    LightPack& lightPack = scene.lightPack;
    
    if(lightPack.lights.size() == 0){
        showLightPanel = false;
        return;
    }
    
    LightInfo& light = lightPack.lights[lightPack.selected];
    
    ImGui::SetNextWindowSizeConstraints(ImVec2(450, 450), ImVec2(FLT_MAX, FLT_MAX));
    
    ImGui::SetNextWindowPos(ImVec2(windowWidth, windowHeight), ImGuiCond_Always, ImVec2(1.0f, 1.0f));

    ImGui::Begin("Light", &showLightPanel, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    
    if(ImGui::BeginTable("Table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)){
        
        ImGui::TableNextRow();
        
        ImGui::TableSetColumnIndex(0);
        Utilities::SetTableBgColor(column0BgColor);
        
        Utilities::RenderText("Selected Light");
        Utilities::RenderText("Position");
        Utilities::RenderText("Color");
        Utilities::RenderText("Intensity");
        Utilities::RenderText("Minimum Light Intensity");
        Utilities::RenderText("Speed");
        Utilities::RenderText("Near Clipping Plane");
        Utilities::RenderText("Far Clipping Plane");
        Utilities::RenderText("Min Shadow Bias");
        Utilities::RenderText("Max Shadow Bias");
        Utilities::RenderText("Shadow Bias");
        Utilities::RenderText("Shadow Samples");
        Utilities::RenderText("Sampling Scale");
        Utilities::RenderText("Reset Light");
        Utilities::RenderText("Delete Light");
        
        ImGui::TableSetColumnIndex(1);
        Utilities::SetTableBgColor(column1BgColor);
        
        Utilities::RenderDropDownList("##SelectedLight", lightPack.selected, lightPack.namesPtr, 120.0f);
                
        if(Utilities::RenderDragFloat3("##lightPosition", light.config.position.x, ImGui::GetColumnWidth(), 0.1f, 0.0f, 0.0f, "%.2f")){
            //Light::Move(context, scene.modelPack, scene.lightPack, light);
            //scene.modelPack.fsData.lightPositions[lightPack.selected] = light.config.position;
            //light.boundingSphere = Light::ComputeBoundingSphere(light);
            //LightCulling::UpdateLightBoundingSphere(scene, scene.cameraPack.cameras[scene.cameraPack.selected], lightPack.selected);
        }
            
        Utilities::RenderColorEdit("##Color", light.config.color, 120.0f);
        
        if(Utilities::RenderDragFloat("##Intensity", light.config.intensity, ImGui::GetColumnWidth(), 0.1f, 0.1f, 10.0f, "%.2f")){
            //light.boundingSphere = Light::ComputeBoundingSphere(light);
            //scene.modelPack.fsData.lightColors[lightPack.selected] = lightPack.lights[lightPack.selected].config.intensity * lightPack.lights[lightPack.selected].config.color;
            //LightCulling::UpdateLightBoundingSphere(scene, scene.cameraPack.cameras[scene.cameraPack.selected], lightPack.selected);
        }
        
        if(Utilities::RenderSliderFloat("##minLightIntensity", light.config.minLightIntensityPercentage, 1.0f, 100.0f, "%.0f", ImGui::GetColumnWidth())){
            //light.boundingSphere = Light::ComputeBoundingSphere(light);
            //LightCulling::UpdateLightBoundingSphere(scene, scene.cameraPack.cameras[scene.cameraPack.selected], lightPack.selected);
        }

        
        Utilities::RenderDragFloat("##speed", light.config.speed, ImGui::GetColumnWidth(), 0.1f, 0.1f, 10.0f, "%.2f");
        
        if(Utilities::RenderDragFloat("##nearClippingPlane", light.config.nearClippingPlane, ImGui::GetColumnWidth(), 0.1f, 0.1f, 10.0f, "%.2f")){
            //Light::UpdateProjectionMatrix(light);
            //Shadow::Bake(context, scene.modelPack, light, scene.lightPack);
        }
        
        if(Utilities::RenderDragFloat("##farClippingPlane", light.config.farClippingPlane, ImGui::GetColumnWidth(), 0.1f, 0.1f, 10.0f, "%.2f")){
            //Light::UpdateProjectionMatrix(light);
            //Shadow::Bake(context, scene.modelPack, light, scene.lightPack);
        }
        
        Utilities::RenderDragFloat("##shadowBiasMin", light.config.minShadowBias, ImGui::GetColumnWidth(), 0.00001f, 0.0, 1.0f, "%.6f");

        Utilities::RenderDragFloat("##shadowBiasMax", light.config.maxShadowBias, ImGui::GetColumnWidth(), 0.00001f, 0.0, 1.0f, "%.6f");

        Utilities::RenderSliderFloat("##shadowBias", light.config.shadowBias, light.config.minShadowBias, light.config.maxShadowBias, "%.6f", ImGui::GetColumnWidth());
        
        Utilities::RenderSliderInt("##shadowNumSamples", light.config.numShadowSamples, 1, maxNumShadowSamples, ImGui::GetColumnWidth());
        
        Utilities::RenderDragFloat("##softShadowSampleDirectionsOffset", light.config.softShadowSampleDirectionsOffset, ImGui::GetColumnWidth(), 0.1f, 0.000001, 1.0f, "%.6f");
                        
        if(Utilities::RenderButton("Reset Light", 120.0f))
            ;//Light::Reset(context, scene.modelPack, scene.lightPack, light);
        
        if(Utilities::RenderButton("Delete Light", 120.0f))
            ;//Light::Delete(context, scene, lightPack);

        ImGui::EndTable();
    }
    
    ImGui::End();
}
