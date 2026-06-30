#pragma once
#include "ActionInfo.h"
#include "../../../Ui/Text/Text.h"
#include "../../Variable/Variable.h"
#include "../../../Scene/Light/Light.h"
#include "../../../Scene/Camera/Camera.h"


#define CAMERA 0
    #define SWITCH 0
    #define MOVE 1
        #define FORWARD 0
        #define BACKWARD 1
        #define RIGHTWARD 2
        #define LEFTWARD 3
        #define UPWARD 4
        #define DOWNWARD 5
    #define LOOK_AROUND 2
        #define UP 0
        #define DOWN 1
        #define RIGHT 2
        #define LEFT 3

std::array<void(*)(CameraInfo&, SceneInfo&), 4> cameraLookAroundActionHandlers{
    [](CameraInfo& camera, SceneInfo& scene){
        Camera::LookAround(camera, camera.right, 1.0f, -30.0f, 30.0f, -30.0f, 50.0f, scene);
    },
    [](CameraInfo& camera, SceneInfo& scene){
        Camera::LookAround(camera, camera.right, -1.0f, -30.0f, 30.0f, -30.0f, 50.0f, scene);
    },
    [](CameraInfo& camera, SceneInfo& scene){
        Camera::LookAround(camera, camera.up, -1.0f, -30.0f, 30.0f, -30.0f, 50.0f, scene);
    },
    [](CameraInfo& camera, SceneInfo& scene){
        Camera::LookAround(camera, camera.up, 1.0f, -30.0f, 30.0f, -30.0f, 50.0f, scene);
    }
};

std::array<void(*)(CameraInfo&, SceneInfo&), 6> cameraMoveActionHandlers{
    [](CameraInfo& camera, SceneInfo& scene){
        Camera::Move(camera, camera.config.direction, scene);
    },
    [](CameraInfo& camera, SceneInfo& scene){
        Camera::Move(camera, -camera.config.direction, scene);
    },
    [](CameraInfo& camera, SceneInfo& scene){
        Camera::Move(camera, camera.right, scene);
    },
    [](CameraInfo& camera, SceneInfo& scene){
        Camera::Move(camera, -camera.right, scene);
    },
    [](CameraInfo& camera, SceneInfo& scene){
        Camera::Move(camera, camera.up, scene);
    },
    [](CameraInfo& camera, SceneInfo& scene){
        Camera::Move(camera, -camera.up, scene);
    }
};

std::array<void(*)(CameraPack&, SceneInfo&, ActionInfo&), 3> cameraActionHandlers
{
    [](CameraPack& cameraPack, SceneInfo& scene, ActionInfo& action)
    {
        if(std::holds_alternative<int>(action.value))
            Camera::Switch(cameraPack, scene, std::get<int>(action.value));
    },
    
    [](CameraPack& cameraPack, SceneInfo& scene, ActionInfo& action)
    {
        cameraMoveActionHandlers[action.subType](cameraPack.cameras[cameraPack.selected], scene);
    },
    
    [](CameraPack& cameraPack, SceneInfo& scene, ActionInfo& action)
    {
        cameraLookAroundActionHandlers[action.subType](cameraPack.cameras[cameraPack.selected], scene);
    }
};





#define TEXT 1
    #define DISPLAY 0
    #define HIDE 1
    #define HOVER_PREV 2
    #define HOVER_NEXT 3
    #define HOVER_UNCONDITIONALY 4
    #define HOVER_CONDITIONALY 5
    #define MAKE_SILENT_ALL 6
    #define MAKE_SOUND_ALL 7

std::array<void(*)(Ui::TextPack&, uint&), 8> textActionHandlers{
    
    // DISPLAY
    [](Ui::TextPack& textPack, uint& textIndex){
        Ui::Text::Display(textPack, textIndex);
    },
    
    // HIDE
    [](Ui::TextPack& textPack, uint& textIndex){
        Ui::Text::Hide(textPack, textIndex);
    },
    
    // HOVER_PREV
    [](Ui::TextPack& textPack, uint& textIndex){
        Ui::Text::HoverPrevious(textPack);
    },
    
    // HOVER_NEXT
    [](Ui::TextPack& textPack, uint& textIndex){
        Ui::Text::HoverNext(textPack);
    },

    // HOVER_UNCONDITIONALY
    [](Ui::TextPack& textPack, uint& textIndex){
        textPack.texts[textIndex].config.hoverUnconditionally = true;
    },
    
    // HOVER_CONDITIONALY
    [](Ui::TextPack& textPack, uint& textIndex){
        textPack.texts[textIndex].config.hoverUnconditionally = false;
    },
    
    // MAKE_SILENT_ALL
    [](Ui::TextPack& textPack, uint& textIndex){
        for(size_t i = 0; i < textPack.texts.size(); i++)
            textPack.texts[i].config.silentWhenProgressing = true;
    },
    
    // MAKE_SOUND_ALL
    [](Ui::TextPack& textPack, uint& textIndex){
        for(size_t i = 0; i < textPack.texts.size(); i++)
            textPack.texts[i].config.silentWhenProgressing = false;
    }
};





#define MODEL 2
    #define TRANSLATE 0
    #define ROTATE 1
    #define RESET_TRANSFORMATION 2

std::array<void(*)(ContextInfo&, SceneInfo&, ActionInfo&), 3> modelActionHandlers{
    [](ContextInfo& context, SceneInfo& scene, ActionInfo& action) {
        if(std::holds_alternative<glm::vec3>(action.value))
            Transform::Translate(context, scene.modelPack, scene.lightPack, action.target, std::get<glm::vec3>(action.value));
    },
    [](ContextInfo& context, SceneInfo& scene, ActionInfo& action) {
        if(std::holds_alternative<glm::vec3>(action.value))
            Transform::Rotate(context, scene.modelPack, scene.lightPack, action.target, std::get<glm::vec3>(action.value));
    },
    [](ContextInfo& context, SceneInfo& scene, ActionInfo& action) {
        Transform::Reset(context, scene.modelPack, scene.lightPack, action.target);
    }
};





#define STATE 3
    #define TRANSITION 0

std::array<void(*)(StatePack&, ScenePack&, ActionInfo&), 1> stateActionHandlers{
    [](StatePack& statePack, ScenePack& scenePack, ActionInfo& action){
        State::Transition(statePack, scenePack, action.target);
    }
};





#define SYSTEM 4
    #define EXIT 0

std::array<void(*)(GLFWwindow *), 1> systemActionHandlers{
    [](GLFWwindow * window){
        glfwSetWindowShouldClose(window, true);
    }
};





#define UI 5
    #define OPEN 0
    #define CLOSE 1
    #define CHANGE_COLOR 2

std::array<void(*)(Ui::ManagerInfo&, ActionInfo&), 3> uiActionHandlers{
    [](Ui::ManagerInfo& ui, ActionInfo& action){
        ui.elements[action.target].flag = true;
    },
    [](Ui::ManagerInfo& ui, ActionInfo& action){
        ui.elements[action.target].flag = false;
    },
    [](Ui::ManagerInfo& ui, ActionInfo& action){
        ui.elements[action.target].selectedColor = std::get<int>(action.value);
    }
};




#define SCENE 6
    #define DISABLE_RENDERING 0
    #define ENABLE_RENDERING 1
    #define CHANGE_BACKGROUND_COLOR 2

std::array<void(*)(SceneInfo&, ActionInfo&), 3> sceneActionHandlers{
    [](SceneInfo& scene, ActionInfo& action){
        scene.enableRendering = false;
    },
    [](SceneInfo& scene, ActionInfo& action){
        scene.enableRendering = true;
    },
    [](SceneInfo& scene, ActionInfo& action){
        Scene::SetBackgroundColor(scene, std::get<glm::vec3>(action.value));
    }
};



#define VARIABLE 7
    #define CHANGE_VALUE 0

std::array<void(*)(VariableInfo&, ActionInfo&), 1> variableActionHandlers{
    [](VariableInfo& variable, ActionInfo& action){
        Variable::ChangeValue(variable, action);
    }
};









std::array<void(*)(ContextInfo&, ScenePack&, StatePack&, Ui::ManagerInfo&, Ui::TextPack&, GLFWwindow *, std::vector<VariableInfo>&, ActionInfo&), 8> actionHandlers{
    [](ContextInfo& context, ScenePack& scenePack, StatePack& statePack, Ui::ManagerInfo& ui, Ui::TextPack& textPack, GLFWwindow * window, std::vector<VariableInfo>& variables, ActionInfo& action){
        cameraActionHandlers[action.type](scenePack.scenes[scenePack.selected].cameraPack, scenePack.scenes[scenePack.selected], action);
    },
    [](ContextInfo& context, ScenePack& scenePack, StatePack& statePack, Ui::ManagerInfo& ui, Ui::TextPack& textPack, GLFWwindow * window, std::vector<VariableInfo>& variables, ActionInfo& action){
        textActionHandlers[action.type](textPack, action.target);
    },
    [](ContextInfo& context, ScenePack& scenePack, StatePack& statePack, Ui::ManagerInfo& ui, Ui::TextPack& textPack, GLFWwindow * window, std::vector<VariableInfo>& variables, ActionInfo& action){
        modelActionHandlers[action.type](context, scenePack.scenes[scenePack.selected], action);
    },
    [](ContextInfo& context, ScenePack& scenePack, StatePack& statePack, Ui::ManagerInfo& ui, Ui::TextPack& textPack, GLFWwindow * window, std::vector<VariableInfo>& variables, ActionInfo& action){
        stateActionHandlers[action.type](statePack, scenePack, action);
    },
    [](ContextInfo& context, ScenePack& scenePack, StatePack& statePack, Ui::ManagerInfo& ui, Ui::TextPack& textPack, GLFWwindow * window, std::vector<VariableInfo>& variables, ActionInfo& action){
        systemActionHandlers[action.type](window);
    },
    [](ContextInfo& context, ScenePack& scenePack, StatePack& statePack, Ui::ManagerInfo& ui, Ui::TextPack& textPack, GLFWwindow * window, std::vector<VariableInfo>& variables, ActionInfo& action){
        uiActionHandlers[action.type](ui, action);
    },
    [](ContextInfo& context, ScenePack& scenePack, StatePack& statePack, Ui::ManagerInfo& ui, Ui::TextPack& textPack, GLFWwindow * window, std::vector<VariableInfo>& variables, ActionInfo& action){
        sceneActionHandlers[action.type](scenePack.scenes[scenePack.selected], action);
    },
    [](ContextInfo& context, ScenePack& scenePack, StatePack& statePack, Ui::ManagerInfo& ui, Ui::TextPack& textPack, GLFWwindow * window, std::vector<VariableInfo>& variables, ActionInfo& action){
        variableActionHandlers[action.type](variables[action.target], action);
    }
};





static void TriggerActions(ContextInfo& context, ScenePack& scenePack, StatePack& statePack, Ui::ManagerInfo& ui, Ui::TextPack& textPack, GLFWwindow * window, std::vector<VariableInfo>& variables, EventInfo& event)
{
    for(size_t j = 0; j < event.actions.size(); j++)
        actionHandlers[event.actions[j].entity](context, scenePack, statePack, ui, textPack, window, variables, event.actions[j]);
}





#define KEYBOARD 0
    #define PRESS 0
    #define HOLD 1
#define FLAG_CHANGE 1
#define TEXT_CLICK 2
#define TEXT_ENTER 3

std::array<void(*)(ContextInfo&, ScenePack&, StatePack&, Ui::ManagerInfo&, Ui::TextPack&, GLFWwindow *, std::vector<VariableInfo>&, EventInfo&), 4> eventHandlers{
    [](ContextInfo& context, ScenePack& scenePack, StatePack& statePack, Ui::ManagerInfo& ui, Ui::TextPack& textPack, GLFWwindow * window, std::vector<VariableInfo>& variables, EventInfo& event){
        if(event.keyPressType == PRESS){
            bool keyState = glfwGetKey(window, event.glfwKey) == GLFW_PRESS;
            if(keyState && !event.previousKeyState)
                TriggerActions(context, scenePack, statePack, ui, textPack, window, variables, event);
            event.previousKeyState = keyState;
        }
        else if(event.keyPressType == HOLD){
            if(glfwGetKey(window, event.glfwKey) == GLFW_PRESS)
                TriggerActions(context, scenePack, statePack, ui, textPack, window, variables, event);
        }
    },
    [](ContextInfo& context, ScenePack& scenePack, StatePack& statePack, Ui::ManagerInfo& ui, Ui::TextPack& textPack, GLFWwindow * window, std::vector<VariableInfo>& variables, EventInfo& event){
        Ui::ElementInfo& element = ui.elements[event.target];
        if(element.oldFlag == std::get<bool>(event.oldValue)){
            if(element.flag == std::get<bool>(event.newValue))
                TriggerActions(context, scenePack, statePack, ui, textPack, window, variables, event);
        }
    },
    [](ContextInfo& context, ScenePack& scenePack, StatePack& statePack, Ui::ManagerInfo& ui, Ui::TextPack& textPack, GLFWwindow * window, std::vector<VariableInfo>& variables, EventInfo& event){
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
            if(textPack.selected == event.target){
                textPack.selected = -1;
                Ui::TextInfo& text = textPack.texts[event.target];
                text.config.color = text.config.originalColor;
                TriggerActions(context, scenePack, statePack, ui, textPack, window, variables, event);
            }
        }
    },
    [](ContextInfo& context, ScenePack& scenePack, StatePack& statePack, Ui::ManagerInfo& ui, Ui::TextPack& textPack, GLFWwindow * window, std::vector<VariableInfo>& variables, EventInfo& event){
        if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS){
            if(textPack.selected == event.target){
                textPack.selected = -1;
                Ui::TextInfo& text = textPack.texts[event.target];
                text.config.color = text.config.originalColor;
                TriggerActions(context, scenePack, statePack, ui, textPack, window, variables, event);
            }
        }
    }
};




std::unordered_map<std::string, int> glfwKeys{
    {"A", GLFW_KEY_A},
    {"W", GLFW_KEY_W},
    {"C", GLFW_KEY_C},
    {"U", GLFW_KEY_U},
    {"S", GLFW_KEY_S},
    {"D", GLFW_KEY_D},
    {"Q", GLFW_KEY_Q},
    {"E", GLFW_KEY_E},
    {"RIGHT", GLFW_KEY_RIGHT},
    {"LEFT", GLFW_KEY_LEFT},
    {"UP", GLFW_KEY_UP},
    {"DOWN", GLFW_KEY_DOWN},
    {"Esc", GLFW_KEY_ESCAPE},
    {"G", GLFW_KEY_G}
};

std::unordered_map<std::string, int> actionEntities{
    {"MODEL", MODEL},
    {"CAMERA", CAMERA},
    {"TEXT", TEXT},
    {"STATE", STATE},
    {"SYSTEM", SYSTEM},
    {"UI", UI},
    {"SCENE", SCENE},
    {"VARIABLE", VARIABLE}
};

std::unordered_map<std::string, int> actionTypes{
    {"SWITCH", SWITCH},
    {"MOVE", MOVE},
    {"LOOK_AROUND", LOOK_AROUND},
    {"RIGHTWARD", RIGHTWARD},
    {"LEFTWARD", LEFTWARD},
    {"UPWARD", UPWARD},
    {"DOWNWARD", DOWNWARD},
    {"FORWARD", FORWARD},
    {"BACKWARD", BACKWARD},
    {"RIGHT", RIGHT},
    {"LEFT", LEFT},
    {"UP", UP},
    {"DOWN", DOWN},
    {"DISPLAY", DISPLAY},
    {"HIDE", HIDE},
    {"TRANSLATE", TRANSLATE},
    {"ROTATE", ROTATE},
    {"RESET_TRANSFORMATION", RESET_TRANSFORMATION},
    {"TRANSITION", TRANSITION},
    {"EXIT", EXIT},
    {"OPEN", OPEN},
    {"CLOSE", CLOSE},
    {"HOVER_NEXT", HOVER_NEXT},
    {"HOVER_PREV", HOVER_PREV},
    {"DISABLE_RENDERING", DISABLE_RENDERING},
    {"ENABLE_RENDERING", ENABLE_RENDERING},
    {"CHANGE_BACKGROUND_COLOR", CHANGE_BACKGROUND_COLOR},
    {"CHANGE_VALUE", CHANGE_VALUE},
    {"HOVER_UNCONDITIONALY", HOVER_UNCONDITIONALY},
    {"HOVER_CONDITIONALY", HOVER_CONDITIONALY},
    {"MAKE_SILENT_ALL", MAKE_SILENT_ALL},
    {"MAKE_SOUND_ALL", MAKE_SOUND_ALL},
    {"CHANGE_COLOR", CHANGE_COLOR}
};
