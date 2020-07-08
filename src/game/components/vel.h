#pragma once

#include "game/main.h"

#include "game/components/pos.h"

namespace Components
{
    struct Vel : Ent::Component, Ent::Requires<Pos>
    {
        fvec2 vel{};

        Vel() {}
        Vel(ivec2 vel) : vel(vel) {}
    };

    inline const auto e_with_pos_and_vel = EntitiesConfig().AddList(Ent::has_components<Pos, Vel>);
}
