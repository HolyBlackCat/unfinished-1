#include <iomanip>
#include <iostream>
#include <vector>

#include "graphics/complete.h"
#include "input/complete.h"
#include "interface/messagebox.h"
#include "interface/window.h"
#include "program/errors.h"
#include "program/exit.h"
#include "program/parachute.h"
#include "reflection/complete.h"
#include "utils/adjust.h"
#include "utils/archive.h"
#include "utils/clock.h"
#include "utils/dynamic_storage.h"
#include "utils/finally.h"
#include "utils/json.h"
#include "utils/macro.h"
#include "utils/mat.h"
#include "utils/memory_file.h"
#include "utils/meta.h"
#include "utils/metronome.h"
#include "utils/random.h"
#include "utils/resource_allocator.h"
#include "utils/robust_compare.h"
#include "utils/strings.h"
#include "utils/tasks.h"

#include "game/adaptive_viewport.h"
#include "game/map.h"
#include "game/render.h"

#define main SDL_main

ivec2 screen_size = ivec2(1920,1080)/4;

Program::Parachute error_parachute;
Interface::Window win("Gamma", screen_size*2);
Graphics::DummyVertexArray dummy_vao;

Random random;

Render render(1000, Graphics::ShaderConfig::Core());
Graphics::Texture tex = Graphics::Texture().SetData(Graphics::Image("assets/texture.png")).Interpolation(Graphics::nearest).Wrap(Graphics::clamp);
AdaptiveViewport adaptive_viewport(Graphics::ShaderConfig::Core(), screen_size);

Input::Mouse mouse;

int main(int, char**)
{
    Map::TileSheet sheet("assets/tile_sheet.txt");
    Map::Format format;
    format.tile_layers.push_back(Map::TileLayerFormat("mid", &sheet));
    Map map(format, "assets/map.json");

    { // Autotiling
        Map::TileLayer &la = map.Layer(0);

        for (int y = 0; y < la.Size().y; y++)
        for (int x = 0; x < la.Size().x; x++)
        {
            int index = la.UnsafeGet(ivec2(x,y)).index;
            if (index == Map::index_none)
                continue;

//            bool a = index == la.ClampGet(ivec2(x,y-1)).index;
//            bool b = index == la.ClampGet(ivec2(x+1,y)).index;
//            bool c = index == la.ClampGet(ivec2(x,y+1)).index;
//            bool d = index == la.ClampGet(ivec2(x-1,y)).index;
//            bool ab = index == la.ClampGet(ivec2(x+1,y-1)).index;
//            bool bc = index == la.ClampGet(ivec2(x+1,y+1)).index;
//            bool cd = index == la.ClampGet(ivec2(x-1,y+1)).index;
//            bool da = index == la.ClampGet(ivec2(x-1,y-1)).index;

            std::string ret;

//            if (!a && !b && !c && !d)
//                ret = 3;
//            else if (a && !b && c && !d)
//                ret = 7;
//            else if (!a && b && !c && d)
//                ret = 11;
//            else if (!a && !b && c && cd && d)
//                ret = 2;
//            else if (!b && !c && d && da && a)
//                ret = 10;
//            else if (!c && !d && a && ab && a)
//                ret = 8;
//            else if (!d && !a && b && bc && c)
//                ret = 0;
//            else if (!a && b && bc && c && cd && d)
//                ret = 1;
//            else if (!b && c && cd && d && da && a)
//                ret = 6;
//            else if (!c && d && da && a && ab && b)
//                ret = 9;
//            else if (!d && a && ab && b && bc && c)
//                ret = 4;
//            else if (a && ab && b && bc && c && cd && d && da)
//                ret = 5;
//            else
            ret = Str("*", random.integer() < 2);

            la.TrySetVariantName(ivec2(x,y), ret);
        }

        map.ValidateVariantIndices();
    }


    Graphics::Blending::Enable();
    Graphics::Blending::FuncNormalPre();

    render.SetTexture(tex);
    adaptive_viewport.Update();

    render.SetMatrix(adaptive_viewport.GetDetails().MatrixCentered());
    mouse.SetMatrix(adaptive_viewport.GetDetails().MouseMatrixCentered());

    Clock::DeltaTimer delta_timer;
    Metronome metronome;
    while (1)
    {
        uint64_t delta = delta_timer();
        while (metronome.Tick(delta))
        {
            win.ProcessEvents();

            if (win.Resized())
            {
                std::cout << "Resized\n";
                adaptive_viewport.Update();
                mouse.SetMatrix(adaptive_viewport.GetDetails().MouseMatrixCentered());
            }
            if (win.ExitRequested())
                return 0;
        }

        adaptive_viewport.BeginFrame();
        Graphics::Clear();
        render.BindShader();
        render.iquad(-screen_size/2, screen_size).color(fvec3(0));
//        render.fquad(mouse.pos(), fvec2(32)).center().color(fvec3(1,0.5,0));
//        render.fquad(mouse.pos().add_y(32), fvec2(16)).center().tex(fvec2(16,0));
        map.Layer(0).Render(render, screen_size, mouse.pos());
        render.Finish();
        adaptive_viewport.FinishFrame();
        Graphics::CheckErrors();

        win.SwapBuffers();
    }

    return 0;
}
