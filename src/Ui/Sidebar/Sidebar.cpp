#include "Sidebar.h"
#include "../Constants.h"
#include "../Utilities/Utilities.h"



void Ui::Sidebar::Render(ScenePack& pack, int windowWidth, int windowHeight)
{
    ImGui::SetNextWindowSizeConstraints(ImVec2(20, 100), ImVec2(FLT_MAX, FLT_MAX));
    
    ImGui::SetNextWindowPos(ImVec2(windowWidth, windowHeight/2.0f), ImGuiCond_Always, ImVec2(1.0f, 0.5f));
    
    ImGui::Begin("Sidebar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
    
    if(Ui::Utilities::RenderButton(ICON_FA_PEN, ImGui::GetColumnWidth()) && (pack.scenes.size() > 0) && ( (pack.scenes[pack.selected].modelPack.models.size() > 0) || (pack.scenes[pack.selected].lightPack.lights.size() > 0) || (pack.scenes[pack.selected].cameraPack.cameras.size() > 0) ) ){
        showEditingModePanel = true;
        showLightPanel = showModelPanel = showCameraPanel = showSkyboxPanel = showUserGuidePanel = showPerformancePanel = showScenePanel = showOptimizationsPanel = false;
    }
    if(ImGui::IsItemHovered())
        ImGui::SetTooltip("Editing Mode");

    if(Ui::Utilities::RenderButton(ICON_FA_OBJECT_GROUP, ImGui::GetColumnWidth())){
        showScenePanel = true;
        showLightPanel = showEditingModePanel = showModelPanel = showCameraPanel = showSkyboxPanel = showUserGuidePanel = showPerformancePanel = showOptimizationsPanel = false;
    }
    if(ImGui::IsItemHovered())
        ImGui::SetTooltip("Scene");
    
    if(Ui::Utilities::RenderButton(ICON_FA_LIGHTBULB, ImGui::GetColumnWidth()) && (pack.scenes.size() > 0) && (pack.scenes[pack.selected].lightPack.lights.size() > 0)){
        showLightPanel = true;
        showModelPanel = showCameraPanel = showSkyboxPanel = showUserGuidePanel = showScenePanel = showEditingModePanel = showPerformancePanel = showOptimizationsPanel = false;
    }
    if(ImGui::IsItemHovered())
        ImGui::SetTooltip("Light");

    if(Ui::Utilities::RenderButton(ICON_FA_CUBE, ImGui::GetColumnWidth()) && (pack.scenes.size() > 0) && (pack.scenes[pack.selected].modelPack.models.size() > 0)){
        showModelPanel = true;
        showLightPanel = showCameraPanel = showSkyboxPanel = showUserGuidePanel = showScenePanel = showEditingModePanel = showPerformancePanel = showOptimizationsPanel = false;
    }
    if(ImGui::IsItemHovered())
        ImGui::SetTooltip("Model");

    if(Ui::Utilities::RenderButton(ICON_FA_CAMERA, ImGui::GetColumnWidth())){
        showCameraPanel = true;
        showLightPanel = showModelPanel = showSkyboxPanel = showUserGuidePanel = showScenePanel = showEditingModePanel = showPerformancePanel = showOptimizationsPanel = false;
    }
    if(ImGui::IsItemHovered())
        ImGui::SetTooltip("Camera");

    if(Ui::Utilities::RenderButton(ICON_FA_GLOBE, ImGui::GetColumnWidth())){
        showSkyboxPanel = true;
        showLightPanel = showScenePanel = showModelPanel = showCameraPanel = showUserGuidePanel = showEditingModePanel = showPerformancePanel = showOptimizationsPanel = false;
    }
    if(ImGui::IsItemHovered())
        ImGui::SetTooltip("Skybox");

    if(Ui::Utilities::RenderButton(ICON_FA_CHART_BAR, ImGui::GetColumnWidth())){
        showPerformancePanel = true;
        showLightPanel = showScenePanel = showModelPanel = showCameraPanel = showSkyboxPanel = showUserGuidePanel = showEditingModePanel = showOptimizationsPanel = false;
    }
    if(ImGui::IsItemHovered())
        ImGui::SetTooltip("Performance");

    if(Ui::Utilities::RenderButton(ICON_FA_TACHOMETER_ALT, ImGui::GetColumnWidth()) && (pack.scenes.size() > 0) && (pack.scenes[pack.selected].modelPack.models.size() > 0)){
        showOptimizationsPanel = true;
        showLightPanel = showScenePanel = showModelPanel = showCameraPanel = showSkyboxPanel = showUserGuidePanel = showEditingModePanel = showPerformancePanel = false;
    }
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Optimizations");

    ImGui::End();
}
