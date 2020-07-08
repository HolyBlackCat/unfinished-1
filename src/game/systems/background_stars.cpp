#include "game/main.h"

#include "game/components/background_star.h"

static auto e_stars = EntitiesConfig().AddList(Ent::has_components<Components::BackgroundStar>);

namespace InitActions
{
    STRUCT( _01_Stars EXTENDS Actions::Init )
    {
        UNNAMED_MEMBERS()
        void init(entity_controller_t &c) const override
        {
            static auto te = c.MakeEntityTemplate<Components::BackgroundStar>();

            for (int i = 0; i < 200; i++)
                c.Create(te);
        }
    };
}

namespace TickActions
{
    STRUCT( _01_Stars EXTENDS Actions::Tick )
    {
        UNNAMED_MEMBERS()
        void tick(entity_controller_t &c, const State::NextStateSelector &) const override
        {
            for (Ent::Entity &e : c(e_stars))
                e.get<Components::BackgroundStar>().Move(mouse.pos_f());
        }
    };
}

namespace RenderActions
{
    STRUCT( _01_Stars EXTENDS Actions::Render )
    {
        UNNAMED_MEMBERS()
        void render(const entity_controller_t &c) const override
        {
            for (Ent::Entity &e : c(e_stars))
                e.get<Components::BackgroundStar>().Render();
        }
    };
}
