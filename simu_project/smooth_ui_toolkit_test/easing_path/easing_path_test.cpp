/**
 * @file cubic_bezier.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-12-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <smooth_ui_toolkit.h>
#include "../../hal/hal.h"
#include "core/easing_path/easing_path.h"
#include "lgfx/v1/misc/enum.hpp"
#include "spdlog/spdlog.h"
#include <mooncake.h>


using namespace SmoothUIToolKit;



// Mirro the y direction 
static int _mirro_y(int y)
{
    return HAL::GetCanvas()->height() - y;
}


void easing_path_simple_test()
{
    int scale = 3;
    int x_offset = (HAL::GetCanvas()->width() - EasingPath::maxT / scale) / 4;
    int y_offset = (HAL::GetCanvas()->height() - EasingPath::maxT / scale) / 2;


    // Draw path curve 
    HAL::GetCanvas()->fillScreen(TFT_WHITE);
    HAL::GetCanvas()->drawRect(x_offset, _mirro_y(y_offset), EasingPath::maxT / scale, -(EasingPath::maxT / scale), TFT_DARKGRAY);
    for (int t = 0; t < EasingPath::maxT; t++)
    {
        // Get value 
        // auto b = EasingPath::linear(t);
        // auto b = EasingPath::easeInQuad(t);
        // auto b = EasingPath::easeOutQuad(t);
        // auto b = EasingPath::easeInOutQuad(t);
        // auto b = EasingPath::easeInCubic(t);
        // auto b = EasingPath::easeOutCubic(t);
        // auto b = EasingPath::easeInOutCubic(t);
        // auto b = EasingPath::easeInQuart(t);
        // auto b = EasingPath::easeOutQuart(t);
        // auto b = EasingPath::easeInOutQuart(t);
        // auto b = EasingPath::easeInQuint(t);
        // auto b = EasingPath::easeOutQuint(t);
        // auto b = EasingPath::easeInOutQuint(t);
        // auto b = EasingPath::easeInSine(t);
        // auto b = EasingPath::easeOutSine(t);
        // auto b = EasingPath::easeInOutSine(t);
        // auto b = EasingPath::easeInExpo(t);
        // auto b = EasingPath::easeOutExpo(t);
        // auto b = EasingPath::easeInOutExpo(t);
        // auto b = EasingPath::easeInCirc(t);
        // auto b = EasingPath::easeOutCirc(t);
        // auto b = EasingPath::easeInOutCirc(t);
        // auto b = EasingPath::easeInBack(t);
        // auto b = EasingPath::easeOutBack(t);
        // auto b = EasingPath::easeInOutBack(t);
        // auto b = EasingPath::easeInElastic(t);
        // auto b = EasingPath::easeOutElastic(t);
        // auto b = EasingPath::easeInOutElastic(t);
        // auto b = EasingPath::easeInBounce(t);
        // auto b = EasingPath::easeOutBounce(t);
        auto b = EasingPath::easeInOutBounce(t);


        // spdlog::info("b({}) = {}", t, b);
        HAL::GetCanvas()->fillSmoothCircle(t / scale + x_offset, _mirro_y(b / scale + y_offset), 2, TFT_BLACK);
    }
    HAL::CanvasUpdate();
}



struct UserDataTest_t
{
    int xOffset = 0;
    int yOffset = 0;
    int radius = 0;

    UserDataTest_t(int xOffset, int yOffset, int radius)
    {
        this->xOffset = xOffset;
        this->yOffset = yOffset;
        this->radius = radius;
    }
};


static void _render_callback(Transition* transition)
{
    // Reder 
    auto user_data = static_cast<UserDataTest_t*>(transition->getUserData());
    HAL::GetCanvas()->fillSmoothCircle(user_data->xOffset, _mirro_y(transition->getValue() + user_data->yOffset), user_data->radius, TFT_BLACK);

    // If finish, invert back 
    if (transition->isFinish())
    {
        transition->setConfig(transition->getEndValue(), transition->getStartValue());
        transition->setDelay(300);
        transition->reset();
        transition->start(HAL::Millis());
    }
}


void easing_path_play_with_transition()
{
    int scale = EasingPath::maxT / (HAL::GetCanvas()->height() / 2);
    int curve_x_offset = (HAL::GetCanvas()->width() - EasingPath::maxT / scale) / 4;
    int curve_y_offset = (HAL::GetCanvas()->height() - EasingPath::maxT / scale) / 2;

    auto y_start = 0;
    auto y_end = EasingPath::maxT / scale;
    auto radius = y_end / 12;
    auto x_offset = y_end + (HAL::GetCanvas()->width() - y_end) / 2 + curve_x_offset - radius;
    auto y_offset = curve_y_offset;

    // Transition list 
    std::vector<Transition> t_list;
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::linear));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeOutSine));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInSine));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInOutSine));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeOutQuad));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInQuad));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInOutQuad));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeOutCubic));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInCubic));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInOutCubic));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeOutQuart));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInQuart));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInOutQuart));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeOutQuint));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInQuint));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInOutQuint));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeOutExpo));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInExpo));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInOutExpo));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeOutCirc));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInCirc));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInOutCirc));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeOutBack));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInBack));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInOutBack));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeOutElastic));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInElastic));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInOutElastic));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeOutBounce));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInBounce));
    t_list.emplace_back(Transition(y_start, y_end, 800, EasingPath::easeInOutBounce));

    // Set user data and callback 
    for (int i = 0; i < t_list.size(); i++)
    {
        t_list[i].setUserData(new UserDataTest_t(x_offset, y_offset, radius));
        t_list[i].setUpdateCallback(_render_callback);
    }


    int current_t = 0;
    bool changed = true;
    while (1)
    {
        // Input 
        if (HAL::GetAnyButton())
        {
            while (HAL::GetAnyButton());

            current_t++;
            if (current_t > t_list.size() - 1)
            {
                current_t = 0;
            }
            changed = true;
        }

        if (changed)
        {
            // spdlog::info("new {}", current_t);
            // spdlog::info("new {}", current_t);
            changed = false;
            t_list[current_t].reset();
            t_list[current_t].start(HAL::Millis());
        }


        HAL::GetCanvas()->fillScreen(TFT_WHITE);

        // Render ball 
        t_list[current_t].update(HAL::Millis());

        // Render easing path curve 
        HAL::GetCanvas()->drawRect(curve_x_offset, _mirro_y(curve_y_offset), EasingPath::maxT / scale, -(EasingPath::maxT / scale), TFT_DARKGRAY);
        for (int t = 0; t < EasingPath::maxT; t++)
        {
            auto b = t_list[current_t].getTransitionPath()(t);
            #ifndef ESP_PLATFORM
            HAL::GetCanvas()->fillSmoothCircle(t / scale + curve_x_offset, _mirro_y(b / scale + curve_y_offset), 2, TFT_BLACK);
            #else
            HAL::GetCanvas()->fillRect(t / scale + curve_x_offset, _mirro_y(b / scale + curve_y_offset), 2, 2, TFT_BLACK);
            #endif
        }

        #ifdef ESP_PLATFORM
        HAL::RenderFpsPanel();
        #endif
        HAL::CanvasUpdate();
    }

}
