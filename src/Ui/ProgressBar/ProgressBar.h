#pragma once
#include "../../Includes.h"
#include "../../Framework/Variable/Variable.h"
#include "Data/ProgressBarInfo.h"



namespace Ui{
    class  ProgressBar{
    public:
        static void Render(ProgressBarInfo& progressBar, std::vector<VariableInfo>& variables);
        static void Load(nlohmann::json& j, std::vector<ProgressBarInfo>& progressBars, float windowWidth, float windowHeight);
    };
}
