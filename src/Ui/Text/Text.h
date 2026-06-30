#pragma once
#include "Data/TextPack.h"
#include "../../Core/Core.h"
#include "../../Framework/Framework.h"

#include "../../Scene/Scene/Data/ScenePack.h"




namespace Ui{
    class  Text{
    private:
        static void Render(ImFont * font, Ui::TextPack& textPack, std::vector<VariableInfo>& variables, int& textIndex, double xpos, double ypos);
        static void Reset(Ui::TextPack& textPack, int textIndex);
        static bool IsTextHovered(Ui::TextInfo& text, double xpos, double ypos);
        static int GetHoverIndex(Ui::TextPack& textPack, double xpos, double ypos);
    public:
        static void Load(ScenePack& scenePack, Ui::TextPack& textPack, WindowInfo& window, nlohmann::json j, ImFont * font);
        static void Render(ImFont * font, Ui::TextPack& textPack, std::vector<VariableInfo>& variables, double xpos, double ypos);
        static void HoverNext(Ui::TextPack& textPack);
        static void HoverPrevious(Ui::TextPack& textPack);
        static void Display(Ui::TextPack& textPack, int textIndex);
        static void Hide(Ui::TextPack& textPack, int textIndex);
    }; 
}
