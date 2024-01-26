/**
 * @file ring_buffer_test.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-01-12
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "../hal/hal.h"
#include "core/easing_path/easing_path.h"
#include "lgfx/v1/misc/enum.hpp"
#include "spdlog/spdlog.h"
#include <cmath>
#include <cstddef>
#include <mooncake.h>
#include <smooth_ui_toolkit.h>
#include <vector>

using namespace SmoothUIToolKit;

void ring_buffer_test()
{
    auto y_offset = HAL::GetCanvas()->height() / 2;

    RingBuffer<float, 666> rb_sin;
    RingBuffer<float, 666> rb_cos;

    rb_sin.allowOverwrite(true);
    rb_cos.allowOverwrite(true);

    float x = 0.00;
    while (1)
    {
        // Push value into buffer
        x += 0.01;
        rb_sin.put(std::sin(5 * x));
        rb_cos.put(std::cos(x));

        spdlog::info("{} {} {}", rb_sin.valueNum(), rb_sin.readIndex(), rb_sin.writeIndex());

        // Render values
        int chart_x = 0;
        float last_value = 0.0;
        HAL::GetCanvas()->fillScreen(TFT_WHITE);
        rb_sin.peekAll([&chart_x, &last_value, y_offset](float value) {
            // // spdlog::info("sin value: {}", value);
            // HAL::GetCanvas()->drawPixel(chart_x, value * 100 + y_offset, TFT_BLUE);
            // chart_x++;

            if (chart_x == 0)
                HAL::GetCanvas()->drawPixel(chart_x, value * 100 + y_offset, TFT_BLUE);
            else
            {
                HAL::GetCanvas()->drawLine(chart_x - 1, last_value * 100 + y_offset, chart_x, value * 100 + y_offset, TFT_BLUE);
                // HAL::GetCanvas()->drawLine(
                //     chart_x - 1, last_value * 100 + y_offset + 1, chart_x, value * 100 + y_offset + 1, TFT_BLUE);
                // HAL::GetCanvas()->drawLine(
                //     chart_x - 1, last_value * 100 + y_offset + 2, chart_x, value * 100 + y_offset + 2, TFT_BLUE);
            }

            chart_x++;
            last_value = value;
        });
        chart_x = 0;
        rb_cos.peekAll([&chart_x, y_offset](float value) {
            // spdlog::info("cos value: {}", value);
            HAL::GetCanvas()->drawPixel(chart_x, value * 100 + y_offset, TFT_RED);
            chart_x++;
        });
        HAL::CanvasUpdate();

        HAL::Delay(2);
    }
}
