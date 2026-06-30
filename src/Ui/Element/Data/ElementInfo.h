#pragma once
#include "ColumnInfo.h"
#include "../../ProgressBar/Data/ProgressBarInfo.h"




namespace Ui{
    struct ElementInfo{
        bool flag = false;
        std::string label;
        bool oldFlag = true;
        int selectedColor = 0;
        bool noCloseOption = false;
        ImGuiTableFlags tableFlags = 0;
        std::vector<ColumnInfo> columns;
        ImGuiWindowFlags windowFlags = 0;
        std::vector<ImVec4> windowColors;
        ImVec2 windowSize = ImVec2(0.4, 0.3);
        ImVec2 windowPivot = ImVec2(0.5, 0.5);
        ImVec2 windowPosition = ImVec2(0.5, 0.5);
        std::vector<ProgressBarInfo> progressBars;
    };
}
