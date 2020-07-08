#include "game/main.h"

namespace TickActions
{
    STRUCT( _99_Debug_ShowEntityCount EXTENDS Actions::Tick )
    {
        UNNAMED_MEMBERS()
        void tick(entity_controller_t &c, const State::NextStateSelector &) const override
        {
            bool open = ImGui::Begin("EntityCount", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_AlwaysAutoResize);
            FINALLY( ImGui::End(); )
            if (open)
            {
                ImGui::TextUnformatted(STR("Entity count: ", (c.GetEntityCount())).c_str());
            }
        }
    };
}
