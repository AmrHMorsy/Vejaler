#include "ScenePanel.h"
#include "../../Constants.h"
#include "../../Utilities/Utilities.h"



void Ui::ScenePanel::Render(ScenePack& scenePack, float windowWidth, float windowHeight)
{
    if(!showScenePanel)
        return;
    
    if(scenePack.scenes.size() == 0){
        showScenePanel = false;
        return;
    }
    
    SceneInfo& scene = scenePack.scenes[scenePack.selected];
    
    ImGui::SetNextWindowSizeConstraints(ImVec2(300, 100), ImVec2(FLT_MAX, FLT_MAX));
    
    ImGui::SetNextWindowPos(ImVec2(windowWidth, windowHeight), ImGuiCond_Always, ImVec2(1.0f, 1.0f));

    ImGui::Begin("Scene", &showScenePanel, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    if(ImGui::BeginTable("Table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)){
        
        ImGui::TableNextRow();
        
        ImGui::TableSetColumnIndex(0);
        Utilities::SetTableBgColor(column0BgColor);
        Utilities::RenderText("Active Scene");
        Utilities::RenderText("Scene Brightness");
        
        ImGui::TableSetColumnIndex(1);
        Utilities::SetTableBgColor(column1BgColor);
        Utilities::RenderDropDownList("##SelectedScene", scenePack.nextSelected, scenePack.namesPtr, 120.0f);
        
        Utilities::RenderSliderFloat("##sceneBrightness", scene.brightness, 1.0f, 10.0f, "%.6f", ImGui::GetColumnWidth());
        ImGui::EndTable();
    }
    
    ImGui::End();
}
