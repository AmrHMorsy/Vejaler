#pragma once
#include "Data/ElementInfo.h"
#include "../Manager/Data/ManagerInfo.h"
#include "../../Scene/Model/Data/ModelPack.h"
#include "../../Core/Window/Data/WindowInfo.h"
#include "../../Framework/Variable/Data/VariableInfo.h"



namespace Ui{
    class Element{
    private:
        static bool Load(WindowInfo& window, nlohmann::json& e, RowInfo& rowData);
        static bool Load(WindowInfo& window, nlohmann::json& c, ColumnInfo& columnData);
        static bool Load(WindowInfo& window, nlohmann::json& e, Ui::ElementInfo& element);
    public:
        static void Load(WindowInfo& window, Ui::ManagerInfo& ui, nlohmann::json& j);
        static void Render(Ui::ElementInfo& element, ModelPack& modelPack, std::vector<VariableInfo>& variables);
    };
}
