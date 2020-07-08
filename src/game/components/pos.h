#pragma once

#include "game/main.h"

namespace Components
{
    struct Pos : Ent::Component
    {
        fvec2 pos{};

        Pos(ivec2 pos) : pos(pos) {}
    };
}
