/**
 * @file smooth_point_test.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-12-30
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "../hal/hal.h"
#include "core/easing_path/easing_path.h"
#include "lgfx/v1/misc/enum.hpp"
#include "spdlog/spdlog.h"
#include <cstdint>
#include <mooncake.h>
#include <smooth_ui_toolkit.h>
#include <vector>

using namespace SmoothUIToolKit;

void smooth_point_simple_test()
{
    // Point list
    std::vector<Vector2D_t> p_list = {
        Vector2D_t(50, 25),
        Vector2D_t(200, 555),
        Vector2D_t(99, 333),
        Vector2D_t(-35, 111),
        Vector2D_t(222, 24),
        Vector2D_t(35, -2333),
        Vector2D_t(432, 234),
        Vector2D_t(35, 111),
    };

    Transition2D p(p_list[0]);

    p.jumpTo(100, 233);
    p.moveTo(666, 777);

    p.setUpdateCallback([](Transition2D* sp) { spdlog::info("at ({}, {})", sp->getValue().x, sp->getValue().y); });

    int current = 0;
    while (1)
    {
        p.update(HAL::Millis());

        // If finish
        if (p.isFinish())
        {
            current++;
            if (current > p_list.size() - 1)
                current = 0;

            p.moveTo(p_list[current]);
        }

        // Render
        HAL::GetCanvas()->fillScreen(TFT_WHITE);
        HAL::GetCanvas()->fillSmoothCircle(p.getValue().x, p.getValue().y, 10, TFT_BLACK);
        HAL::CanvasUpdate();
    }
}

#include <random>
std::random_device rd;
std::mt19937 gen(rd());
static int _random(int low, int high)
{
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}

static std::vector<Transition2D> sp_list;
static std::vector<int> radius_list;
static std::vector<int> color_list;

void smooth_point_bubble_pool_test()
{
    // int bubble_num = 1024;
    // int bubble_num = 4096;
    int bubble_num = 4096 * 8;
    // int bubble_num = 4096 * 16;
    int min_duration = 200;
    int max_duration = 800;
    // int max_duration = 3000;

    // Genarate random bubbles
    for (int i = 0; i < bubble_num; i++)
    {
        Transition2D sp(_random(0, HAL::GetCanvas()->width()), _random(0, HAL::GetCanvas()->height()));
        // sp.setTransitionPath(EasingPath::easeOutBack);
        sp.setTransitionPath(EasingPath::easeOutQuint);
        sp.setDuration(_random(min_duration, max_duration));
        sp_list.emplace_back(sp);
        radius_list.push_back(_random(2, 24));
        color_list.push_back(_random(0, TFT_WHITE));
    }

    bool is_touching = false;
    std::uint32_t current_time = 0;
    std::uint32_t render_time = 0;
    std::uint32_t time_count = 0;
    while (1)
    {
        // Input
        if (HAL::Millis() - time_count > 5)
        {
            HAL::UpdateTouch();
            if (HAL::IsTouching())
            {
                is_touching = true;
                // spdlog::info("tp: ({}, {})", HAL::GetTouchPoint().x, HAL::GetTouchPoint().y);

                // Move to touch point
                for (int i = 0; i < sp_list.size(); i++)
                {
                    sp_list[i].moveTo(HAL::GetTouchPoint().x, HAL::GetTouchPoint().y);
                }
            }

            // If just released
            else if (is_touching)
            {
                is_touching = false;

                // Move to random point
                for (int i = 0; i < sp_list.size(); i++)
                {
                    sp_list[i].moveTo(_random(0, HAL::GetCanvas()->width()), _random(0, HAL::GetCanvas()->height()));
                }
            }

            time_count = HAL::Millis();
        }

        // Render
        current_time = HAL::Millis();
        HAL::GetCanvas()->fillScreen(TFT_WHITE);
        for (int i = 0; i < sp_list.size(); i++)
        {
            // When moveTo() was called, the next update() will reset the transition's time_offset, into the pass in currentTime
            // So if render tooks quite a long time
            // It's necessary to make sure the transtion reset to the correct time offset (current time - redner time)
            sp_list[i].update(current_time - render_time);

            // Update
            sp_list[i].update(current_time);
            // HAL::GetCanvas()->fillSmoothCircle(sp_list[i].getValue().x, sp_list[i].getValue().y, radius_list[i],
            // color_list[i]);
            HAL::GetCanvas()->fillRect(
                sp_list[i].getValue().x, sp_list[i].getValue().y, radius_list[i], radius_list[i], color_list[i]);
            // HAL::GetCanvas()->drawPixel(sp_list[i].getValue().x, sp_list[i].getValue().y, color_list[i]);
        }
        HAL::GetCanvas()->setTextSize(2);
        HAL::GetCanvas()->setCursor(0, 0);
        HAL::GetCanvas()->printf("transition2d num: %d", bubble_num);
        HAL::CanvasUpdate();
        render_time = HAL::Millis() - current_time;
        spdlog::info("render time: {}", render_time);
    }
}
