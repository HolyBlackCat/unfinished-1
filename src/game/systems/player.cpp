#include "game/main.h"

#include "game/components/controls_config.h"
#include "game/components/player.h"

SIMPLE_STRUCT( Atlas
    VERBATIM Atlas() {TextureAtlas().InitRegions(*this, ".png");}
    DECL(Graphics::TextureAtlas::Region)
        player_ship
)
static Atlas atlas;

namespace InitActions
{
    STRUCT( _20_Player EXTENDS Actions::Init )
    {
        UNNAMED_MEMBERS()
        void init(entity_controller_t &c) const override
        {
            static auto te = c.MakeEntityTemplate<Components::Player>();
            c.Create(te, Components::Pos(ivec2(0, screen_size.y/4)));
        }
    };
}

namespace TickActions
{
    constexpr float max_speed = 2, acc = 0.4;
    constexpr int min_dist_to_screen_edge = 16;

    STRUCT( _20_Player EXTENDS Actions::Tick )
    {
        UNNAMED_MEMBERS()
        void tick(entity_controller_t &c, const State::NextStateSelector &) const override
        {
            using namespace Components;

            ControlsConfig &controls_cfg = c.GetOne(e_controls_config).get<ControlsConfig>();

            for (Ent::Entity &p : c.GetAtMostOne(e_player))
            {
                fvec2 control(controls_cfg.right.down() - controls_cfg.left.down(), controls_cfg.down.down() - controls_cfg.up.down());

                fvec2 &pos = p.get<Pos>().pos;
                fvec2 &vel = p.get<Player>().vel;

                for (int m = 0; m < 2; m++)
                {
                    if (control[m])
                    {
                        // Apply controls.
                        clamp_var_abs(vel[m] += control[m], max_speed);
                    }
                    else
                    {
                        // Decelerate if no controls.
                        if (abs(vel[m]) > acc)
                            vel[m] -= sign(vel[m]) * acc;
                        else
                            vel[m] = 0;
                    }

                    // Change position.
                    pos[m] += vel[m];

                    // Clamp position.
                    int max_abs_pos = screen_size[m]/2 - min_dist_to_screen_edge;
                    if (abs(pos[m]) > max_abs_pos)
                    {
                        // Set velocity to zero if close to screen border and moving towards it.
                        if (sign(vel[m]) == sign(pos[m]))
                            vel[m] = 0;

                        // Actually clamp the position.
                        pos[m] = sign(pos[m]) * max_abs_pos;
                    }
                }
            }
        }
    };
}

namespace RenderActions
{
    STRUCT( _20_Player EXTENDS Actions::Render )
    {
        UNNAMED_MEMBERS()
        void render(const entity_controller_t &c) const override
        {
            for (Ent::Entity &e : c.GetAtMostOne(Components::e_player))
            {
                r.fquad(e.get<Components::Pos>().pos, atlas.player_ship).center();
            }
        }
    };
}
