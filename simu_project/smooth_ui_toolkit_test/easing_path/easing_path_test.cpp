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
int _mirro_y(int y)
{
    return HAL::GetCanvas()->height() - y;
}


void easing_path_test()
{
    spdlog::info("cubic bezier test");


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
