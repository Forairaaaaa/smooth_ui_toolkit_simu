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


using namespace SMOOTH_UI_TK;


// Mirro the y direction 
int _mirro_y(int y)
{
    return HAL::GetCanvas()->height() - y;
}


void cubic_bezier_test()
{
    spdlog::info("cubic bezier test");


    int scale = 3;
    int x_offset = (HAL::GetCanvas()->width() - EasingPath::maxT / scale) / 2;
    int y_offset = (HAL::GetCanvas()->height() - EasingPath::maxT / scale) / 4;


    // Draw path curve 
    HAL::GetCanvas()->fillScreen(TFT_WHITE);
    HAL::GetCanvas()->drawRect(x_offset, _mirro_y(y_offset), EasingPath::maxT / scale, -(EasingPath::maxT / scale), TFT_DARKGRAY);
    for (int t = 0; t < EasingPath::maxT; t++)
    {
        // Get value 
        // auto b = EasingPath::easeInQuad(t);
        auto b = EasingPath::easeOutQuad(t);


        spdlog::info("b({}) = {}", t, b);
        HAL::GetCanvas()->fillSmoothCircle(t / scale + x_offset, _mirro_y(b / scale + y_offset), 2, TFT_BLACK);
    }
    HAL::CanvasUpdate();
    HAL::Delay(5000);




    while (1);
}
