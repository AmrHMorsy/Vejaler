#include "EditingModePanel.h"
#include "../../Constants.h"
#include "../../Utilities/Utilities.h"


void Ui::EditingModePanel::Render(SceneInfo& scene, int windowWidth, int windowHeight)
{
    if(!showEditingModePanel)
        return;
    
    std::string& editingMode = scene.editingMode;
    
    ImGui::SetNextWindowSizeConstraints(ImVec2(200, 60), ImVec2(FLT_MAX, FLT_MAX));
    
    ImGui::SetNextWindowPos(ImVec2(windowWidth, windowHeight), ImGuiCond_Always, ImVec2(1.0f, 1.0f));

    ImGui::Begin("Editing Mode", &showEditingModePanel, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    
    if(ImGui::BeginTable("Table", 1, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)){
        ImGui::TableNextRow();
        
        ImGui::TableSetColumnIndex(0);
        Utilities::SetTableBgColor(column0BgColor);
        Utilities::RenderText(editingMode.c_str());
        
        ImGui::EndTable();
    }
    
    ImGui::End();
}
