#pragma once
#include "../../../Scene/Scene/Data/SceneInfo.h"
#include "../../../Application/Data/ApplicationInfo.h"
#include "../../../Framework/Context/Data/ContextInfo.h"



namespace Ui{
    class TexturesPanel{
    public:
        static void Render(ContextInfo& context, SceneInfo& scene, float panelWidth, ApplicationInfo& application);
        static void RenderImage(TexturePack& texture, bool& isAcceptingEvents, ImVec2 buttonSize, int& selectedResolution);
    };
}
