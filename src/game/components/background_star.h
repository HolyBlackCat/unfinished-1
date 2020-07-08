#pragma once

#include "game/main.h"

namespace Components
{
    struct BackgroundStar : Ent::Component
    {
        fvec2 pos{};
        fvec3 color{};
        float distance = 1;

        void RandomizeStyle()
        {
            constexpr float min_dist = 10, max_dist = 50, min_temp = 1000, max_temp = 10000;

            distance = float(min_dist <= rng.real() <= max_dist);
            float t = 1 / ipow(distance, 2) / (1 / ipow(min_dist, 2));
            t *= float(0.8 <= rng.real() <= 1.2);
            clamp_var(t, 0.13, 1);

            auto TemperatureToColor = [](float temp)
            {
                // From: https://tannerhelland.com/2012/09/18/convert-temperature-rgb-algorithm-code.html

                temp /= 100;

                fvec3 ret;

                if (temp <= 66)
                {
                    ret.r = 1;
                }
                else
                {
                    ret.r = temp - 60;
                    ret.r = 329.698727446 * pow(ret.r, -0.1332047592);
                    clamp_var(ret.r /= 255);
                }

                if (temp <= 66)
                {
                    ret.g = temp;
                    ret.g = 99.4708025861 * std::log(ret.g) - 161.1195681661;
                }
                else
                {
                    ret.g = temp - 60;
                    ret.g = 288.1221695283 * pow(ret.g, -0.0755148492);
                }
                clamp_var(ret.g /= 255);

                if (temp >= 66)
                {
                    ret.b = 1;
                }
                else if (temp <= 19)
                {
                    ret.b = 0;
                }
                else
                {
                    ret.b = temp - 10;
                    ret.b = 138.5177312231 * std::log(ret.b) - 305.0447927307;
                    clamp_var(ret.b /= 255);
                }

                return ret;
            };

            color = TemperatureToColor(min_temp <= rng.real() <= max_temp) * t;
        }
        void RandomizePos(int m)
        {
            pos[m] = float(-screen_size[m]/2 <= rng.real() <= screen_size[m]/2);
        }

        BackgroundStar()
        {
            RandomizeStyle();
            RandomizePos(0);
            RandomizePos(1);
        }

        void Move(fvec2 offset)
        {
            pos += offset / distance;

            for (int m = 0; m < 2; m++)
            {
                if (abs(pos[m]) > screen_size[m]/2)
                {
                    RandomizePos(1 - m);
                    pos[m] = -pos[m] + sign(pos[m]) * 0.5;
                }
            }
        }

        void Render()
        {
            r.fquad(pos, fvec2(1)).center().color(color).beta(0);
        }
    };
}
