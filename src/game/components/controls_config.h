#pragma once

#include "game/main.h"

namespace Components
{
    struct ControlsConfig : Ent::Component
    {
        ivec2 pos{};

        Input::Button up = Input::up;
        Input::Button down = Input::down;
        Input::Button left = Input::left;
        Input::Button right = Input::right;
    };

    inline const auto e_controls_config = EntitiesConfig().AddList(Ent::has_components<ControlsConfig>);
}
