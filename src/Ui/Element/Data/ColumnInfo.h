#pragma once
#include "RowInfo.h"



namespace Ui{
    struct ColumnInfo{
        std::vector<RowInfo> rows;
        ImVec4 color = ImVec4(0.2, 0.2, 0.2, 0.2);
    };
}
