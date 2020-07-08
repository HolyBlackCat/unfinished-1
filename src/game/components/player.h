#pragma once

#include "game/main.h"

#include "game/components/pos.h"
#include "game/components/vel.h"

namespace Components
{
    struct Player
        : Ent::Component
        , Ent::Implies<Pos>
        , Ent::ConflictsWith<Vel> // We provide a custom 'velocity' member
    {
        fvec2 vel{};
    };

    inline const auto e_player = EntitiesConfig().AddList(Ent::has_components<Components::Player>);
}
