#include "PerformancePanel.h"
#include "../../Constants.h"
#include "../../Utilities/Utilities.h"



void Ui::PerformancePanel::Render(SceneInfo& scene, int windowWidth, int windowHeight, uint32_t frame)
{
    if(!showPerformancePanel)
        return;
    
    ImGui::SetNextWindowSizeConstraints(ImVec2(500, 100), ImVec2(FLT_MAX, FLT_MAX));
    
    ImGui::SetNextWindowPos(ImVec2(windowWidth, windowHeight), ImGuiCond_Always, ImVec2(1.0f, 1.0f));
    
    ImGui::Begin("Performance", &showPerformancePanel, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
    
    if(ImGui::BeginTable("Table", 2, ImGuiTableFlags_BordersInnerV | ImGuiTableFlags_RowBg)){
        
        ImGui::TableNextRow();
        
        ImGui::TableSetColumnIndex(0);
        Utilities::SetTableBgColor(column0BgColor);
        Utilities::RenderText("FPS");
        Utilities::RenderText("Frame Time");
        Utilities::RenderText("Vertex Count");
        Utilities::RenderText("Face Count");
        Utilities::RenderText("Meshes Count");
        Utilities::RenderText("Visible Meshes Count");
        
        ImGui::TableSetColumnIndex(1);
        Utilities::SetTableBgColor(column1BgColor);
        float fps = ImGui::GetIO().Framerate;
        Utilities::RenderNumber(fps, "%.1f");
        Utilities::RenderNumber((1000.0f/fps), "%.3f ms");
        
        Utilities::RenderNumber(scene.modelPack.totalNumVertices, "%u");
        Utilities::RenderNumber(scene.modelPack.totalNumTriangles, "%u");
        Utilities::RenderNumber(scene.modelPack.totalNumDrawCommands, "%u");
        Utilities::RenderNumber(scene.modelPack.numVisibleDrawCommands[frame], "%u");
        
        ImGui::EndTable();
    }
    
    ImGui::End();
}
