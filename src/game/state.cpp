#include "game/main.h"

namespace States
{
    STRUCT( Initial EXTENDS State::BasicState )
    {
        UNNAMED_MEMBERS()

        entity_controller_t c;

        Initial()
        {
            EntitiesConfig().ConfigureController(c);
            for (const auto &ptr : ActionSequence<Actions::Init>{})
                ptr->init(c);
        }

        void Tick(const State::NextStateSelector &next_state) override
        {
            { // Display action sequences in a debug GUI.
                #ifndef NDEBUG
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
                #endif
            }

            for (const auto &ptr : ActionSequence<Actions::Tick>{})
                ptr->tick(c, next_state);
        }

        void Render() const override
        {
            Graphics::SetClearColor(fvec3(0));
            Graphics::Clear();

            r.BindShader();

            for (const auto &ptr : ActionSequence<Actions::Render>{})
                ptr->render(c);

            r.Finish();
        }
    };
}
