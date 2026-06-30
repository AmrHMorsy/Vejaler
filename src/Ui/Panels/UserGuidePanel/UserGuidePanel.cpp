#include "UserGuidePanel.h"
#include "../../Constants.h"
#include "../../Utilities/Utilities.h"




void Ui::UserGuidePanel::Render(float windowWidth, float windowHeight)
{
    if(!showUserGuidePanel)
        return;
    
    ImGui::SetNextWindowSize(ImVec2(500, 300), ImGuiCond_Once);
    //ImGui::SetNextWindowSizeConstraints(ImVec2(500, 300), ImVec2(FLT_MAX, FLT_MAX));
    ImGui::SetNextWindowPos(ImVec2(windowWidth/2.0f, windowHeight/2.0f), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
    ImGui::Begin("User Guide", &showUserGuidePanel, ImGuiWindowFlags_NoCollapse);
    
    if(ImGui::BeginTable("Table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)){
        
        ImGui::TableNextRow();
        
        ImGui::TableSetColumnIndex(0);
        Utilities::SetTableBgColor(column0BgColor);
        Utilities::RenderText("Press W / S / D / A");
        Utilities::RenderText("Press ↑ / ↓ / → / ←");
        Utilities::RenderText("Press 1");
        Utilities::RenderText("Press 2");
        Utilities::RenderText("Press 3");
        Utilities::RenderText("Press 4");
        Utilities::RenderText("Press 5");
        Utilities::RenderText("Press 6");
        Utilities::RenderText("Press 7");
        Utilities::RenderText("Press 8");
        Utilities::RenderText("Press 9");
        
        ImGui::TableSetColumnIndex(1);
        Utilities::SetTableBgColor(column1BgColor);
        Utilities::RenderText("Move");
        Utilities::RenderText("Look Around");
        Utilities::RenderText("Inspect Car (Front)");
        Utilities::RenderText("Inspect Car (Rear)");
        Utilities::RenderText("Inspect Hood");
        Utilities::RenderText("Inspect Front Left Wheel");
        Utilities::RenderText("Inspect Front Right Wheel");
        Utilities::RenderText("Inspect Rear Left Wheel");
        Utilities::RenderText("Inspect Rear Right Wheel");
        Utilities::RenderText("Inspect Trunk");
        Utilities::RenderText("Inspect Interior");
        
        ImGui::EndTable();
    }
    
    ImGui::End();
}
