#include "game/main.h"

namespace TickActions
{
    STRUCT( _99_Debug_ShowActionLists EXTENDS Actions::Tick )
    {
        UNNAMED_MEMBERS()
        void tick(entity_controller_t &, const State::NextStateSelector &) const override
        {
            bool open = ImGui::Begin("Sequences");
            FINALLY( ImGui::End(); )
            if (open)
            {
                if (ImGui::CollapsingHeader("Init"))
                {
                    for (const auto &ptr : ActionSequence<Actions::Init>{})
                    {
                        ImGui::Bullet();
                        ImGui::TextUnformatted(Refl::Polymorphic::Name(ptr));
                    }
                }
                if (ImGui::CollapsingHeader("Tick"))
                {
                    for (const auto &ptr : ActionSequence<Actions::Tick>{})
                    {
                        ImGui::Bullet();
                        ImGui::TextUnformatted(Refl::Polymorphic::Name(ptr));
                    }
                }
                if (ImGui::CollapsingHeader("Render"))
                {
                    for (const auto &ptr : ActionSequence<Actions::Render>{})
                    {
                        ImGui::Bullet();
                        ImGui::TextUnformatted(Refl::Polymorphic::Name(ptr));
                    }
                }
            }
        }
    };
}
