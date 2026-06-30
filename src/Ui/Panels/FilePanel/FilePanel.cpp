#include "FilePanel.h"
#include "../../Constants.h"
#include "../../../Scene/Scene/Scene.h"
#include "../../../Scene/Camera/Camera.h"
#include "../LightAddPanel/LightAddPanel.h"
#include "../UserGuidePanel/UserGuidePanel.h"
#include "../CameraAddPanel/CameraAddPanel.h"






void Ui::FilePanel::Render(ScenePack& scenePack, ContextInfo& context, int windowWidth, int windowHeight, ApplicationInfo& application)
{
    if (ImGui::BeginMainMenuBar()){
        if(ImGui::BeginMenu("Save")){
            if(ImGui::MenuItem("Save")){
                if(scenePack.savedFilePath == ""){
                    IGFD::FileDialogConfig config;
                    config.path = ".";
                    config.filePathName = "export.json";
                    application.statePack.states[application.statePack.active].isAcceptingEvents = false;
                    ImGuiFileDialog::Instance()->OpenDialog("Save", "Save File As", ".json", config);
                }
                //else
                    //Scene::Save(scenePack, context, scenePack.savedFilePath);
            }
            if(ImGui::MenuItem("Save As")) {
                IGFD::FileDialogConfig config;
                config.path = ".";
                config.filePathName = "export.json";
                application.statePack.states[application.statePack.active].isAcceptingEvents = false;
                ImGuiFileDialog::Instance()->OpenDialog("SaveAs", "Save File As", ".json", config);
            }
             
            ImGui::EndMenu();
        }
        
        if(ImGui::BeginMenu("New")){
            if(ImGui::MenuItem("Scene")){
                Scene::Add(scenePack, context);
                Camera::Add(context, scenePack.scenes[scenePack.selected], context.swapChain.extent);
                //Light::Add(context, scenePack.scenes[scenePack.selected]);
                scenePack.selected--;
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Load")){
            if(ImGui::MenuItem("Scene")){
                IGFD::FileDialogConfig config;
                config.path = ".";
                application.statePack.states[application.statePack.active].isAcceptingEvents = false;
                ImGuiFileDialog::Instance()->OpenDialog("LoadScene", "Choose File", ".json", config);
            }
            if(ImGui::MenuItem("Model")){
                IGFD::FileDialogConfig config;
                config.path = ".";
                application.statePack.states[application.statePack.active].isAcceptingEvents = false;
                ImGuiFileDialog::Instance()->OpenDialog("LoadModel", "Choose File", ".obj", config);
            }
            
            if(ImGui::MenuItem("Skybox")){
                IGFD::FileDialogConfig config;
                config.path = ".";
                application.statePack.states[application.statePack.active].isAcceptingEvents = false;
                ImGuiFileDialog::Instance()->OpenDialog("LoadSkybox", "Choose File", ".hdr", config);
            }
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Add")){
            if(ImGui::MenuItem("Camera"))
                showAddCameraWindow = true;
            
            if(ImGui::MenuItem("Light"))
                showAddLightWindow = true;
            ImGui::EndMenu();
        }
        if(ImGui::BeginMenu("Help")){
            if(ImGui::MenuItem("User Guide"))
                showUserGuidePanel = true;
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    
    std::string filePath;
    
    if(ImGuiFileDialog::Instance()->Display("LoadSkybox",ImGuiWindowFlags_NoCollapse, ImVec2(800, 600))){
        application.statePack.states[application.statePack.active].isAcceptingEvents = true;
        if(ImGuiFileDialog::Instance()->IsOk())
            filePath = ImGuiFileDialog::Instance()->GetFilePathName();
        ImGuiFileDialog::Instance()->Close();
    }
    
    if(ImGuiFileDialog::Instance()->Display("LoadModel",ImGuiWindowFlags_NoCollapse, ImVec2(800, 600))){
        application.statePack.states[application.statePack.active].isAcceptingEvents = true;
        if(ImGuiFileDialog::Instance()->IsOk())
            filePath = ImGuiFileDialog::Instance()->GetFilePathName();
        ImGuiFileDialog::Instance()->Close();
    }
    
    if(ImGuiFileDialog::Instance()->Display("LoadScene",ImGuiWindowFlags_NoCollapse, ImVec2(800, 600))){
        application.statePack.states[application.statePack.active].isAcceptingEvents = true;
        if(ImGuiFileDialog::Instance()->IsOk())
            filePath = ImGuiFileDialog::Instance()->GetFilePathName();
        ImGuiFileDialog::Instance()->Close();
    }
    
    if(ImGuiFileDialog::Instance()->Display("Save", ImGuiWindowFlags_NoCollapse, ImVec2(800, 600))) {
        application.statePack.states[application.statePack.active].isAcceptingEvents = true;
        //if(ImGuiFileDialog::Instance()->IsOk())
            //Scene::Save(scenePack, context, ImGuiFileDialog::Instance()->GetFilePathName());
        ImGuiFileDialog::Instance()->Close();
    }
    
    if(ImGuiFileDialog::Instance()->Display("SaveAs", ImGuiWindowFlags_NoCollapse, ImVec2(800, 600))) {
        application.statePack.states[application.statePack.active].isAcceptingEvents = true;
        //if(ImGuiFileDialog::Instance()->IsOk())
            //Scene::Save(scenePack, context, ImGuiFileDialog::Instance()->GetFilePathName());
        ImGuiFileDialog::Instance()->Close();
    }
    
    //application.requestedAssets.push_back(filePath);
    
    Ui::UserGuidePanel::Render(windowWidth, windowHeight);
    Ui::LightAddPanel::Render(context, scenePack.scenes[scenePack.selected], windowWidth, windowHeight);
    Ui::CameraAddPanel::Render(context, scenePack.scenes[scenePack.selected], context.swapChain.extent, windowWidth, windowHeight);
}
