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

    int scale = 5;
    int x_offset = (HAL::GetCanvas()->width() - 1024 / scale) / 2;
    int y_offset = (HAL::GetCanvas()->height() - 1024 / scale) / 2;

    // Draw path curve 
    HAL::GetCanvas()->fillScreen(TFT_BLACK);
    HAL::GetCanvas()->drawRect(x_offset, _mirro_y(y_offset), 1024 / scale, -(1024 / scale), TFT_DARKGRAY);
    for (int t = 0; t < 1024; t++)
    {
        // auto b = EasingPath::easeOutBack(t);
        // auto b = EasingPath::easeInOutQuint(t);
        auto b = EasingPath::easeInOutCubic(t);

        spdlog::info("b({}) = {} scale: b({}) = {}", t, b, t / scale, b / scale);
        HAL::GetCanvas()->drawPixel(t / scale + x_offset, _mirro_y(b / scale + y_offset), TFT_YELLOW);
    }
    HAL::CanvasUpdate();
    HAL::Delay(5000);
}
