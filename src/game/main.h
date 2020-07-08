#pragma once

#include "game/master.hpp"

inline constexpr ivec2 screen_size(480, 270);
extern Interface::Window window;
extern const Graphics::ShaderConfig shader_config;
extern Interface::ImGuiController gui_controller;

namespace Fonts
{
    namespace Files
    {
        Graphics::FontFile &Main();
    }

    Graphics::Font &Main();
}

Graphics::TextureAtlas &TextureAtlas();
extern Graphics::Texture texture_main;
extern AdaptiveViewport adaptive_viewport;
extern Render r;
extern Input::Mouse mouse;

extern Ent::ControllerConfig &EntitiesConfig();
using entity_controller_t = Ent::Controller<>;

extern Random<> rng;

namespace Actions
{
    STRUCT( Init POLYMORPHIC )
    {
        UNNAMED_MEMBERS()
        virtual void init(entity_controller_t &c) const = 0;
    };

    STRUCT( Tick POLYMORPHIC )
    {
        UNNAMED_MEMBERS()
        virtual void tick(entity_controller_t &c, const State::NextStateSelector &next_state) const = 0;
    };

    STRUCT( Render POLYMORPHIC )
    {
        UNNAMED_MEMBERS()
        virtual void render(const entity_controller_t &c) const = 0;
    };
}
