/**
 * @file smooth_color_test.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-01-21
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "../hal/hal.h"
#include "core/math/math.h"
#include "core/transition3d/transition3d.h"
#include "lgfx/v1/lgfx_fonts.hpp"
#include "lgfx/v1/misc/colortype.hpp"
#include "lgfx/v1/misc/enum.hpp"
#include "spdlog/spdlog.h"
#include <cstdint>
#include <mooncake.h>
#include <smooth_ui_toolkit.h>
#include <vector>

using namespace SmoothUIToolKit;

void smooth_color_test()
{
    std::vector<uint32_t> color_list = {
        0xFF0000,
        0x00FF00,
        0x0000FF,
        0xFFFFFF,
        0x000000,
        0xb1d5c8,
        0xba5140,
        0x779649,
        0x9aa7b1,
        0xdfceb4,
        0xeeead9,
        0xa4abd6,
        0x6ca8af,
    };

    SmoothRGB smooth_color(color_list[0]);

    // Render
    RingBuffer<RGB_t, 1> color_rb;
    color_rb.reSize(HAL::GetCanvas()->width());
    smooth_color.setUpdateCallback(
        [&color_rb](Transition3D* transition3d)
        {
            auto smooth_rgb = static_cast<SmoothRGB*>(transition3d);

            // Background
            HAL::GetCanvas()->fillScreen(Rgb2Hex(smooth_rgb->getCurrentColor()));
            HAL::GetCanvas()->setCursor(0, 0);
            HAL::GetCanvas()->setFont(&fonts::efontCN_24);
            HAL::GetCanvas()->setTextColor(TFT_WHITE, TFT_BLACK);
            HAL::GetCanvas()->printf("HEX: #%06X\nR:   %3d\nG:   %3d\nB:   %3d",
                                     Rgb2Hex(smooth_rgb->getCurrentColor()),
                                     smooth_rgb->getCurrentRed(),
                                     smooth_rgb->getCurrentGreen(),
                                     smooth_rgb->getCurrentBlue());

            // Chart
            color_rb.put(smooth_rgb->getCurrentColor());
            int chart_x = 0;
            color_rb.peekAll(
                [&chart_x](RGB_t value)
                {
                    HAL::GetCanvas()->drawPixel(chart_x, value.red, TFT_RED);
                    HAL::GetCanvas()->drawPixel(chart_x, value.green, TFT_GREEN);
                    HAL::GetCanvas()->drawPixel(chart_x, value.blue, TFT_BLUE);
                    chart_x++;
                });

            HAL::CanvasUpdate();
        });

    uint32_t time_count = time_count = HAL::Millis();
    int current_index = 0;
    while (1)
    {
        // Switch color
        if (HAL::Millis() - time_count > 1500)
        {
            current_index++;
            if (current_index > color_list.size() - 1)
                current_index = 0;

            smooth_color.moveTo(color_list[current_index]);

            time_count = HAL::Millis();
        }

        smooth_color.update(HAL::Millis());
    }
}
