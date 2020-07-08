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
