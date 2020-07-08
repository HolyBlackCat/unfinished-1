#include "game/main.h"

#include "game/components/controls_config.h"

namespace InitActions
{
    STRUCT( _01_ControlsConfig EXTENDS Actions::Init )
    {
        UNNAMED_MEMBERS()
        void init(entity_controller_t &c) const override
        {
            static auto te = c.MakeEntityTemplate<Components::ControlsConfig>();
            c.Create(te);
        }
    };
}
