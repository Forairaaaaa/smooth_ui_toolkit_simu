/**
 * @file smooth_line_chat_test.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-01-15
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "../hal/hal.h"
#include "core/easing_path/easing_path.h"
#include "lgfx/v1/lgfx_fonts.hpp"
#include "lgfx/v1/misc/enum.hpp"
#include "spdlog/spdlog.h"
#include "utils/ring_buffer/ring_buffer.h"
#include <cmath>
#include <mooncake.h>
#include <smooth_ui_toolkit.h>
#include <vector>

using namespace SmoothUIToolKit;
using namespace SmoothUIToolKit::Chart;

RingBuffer<Vector2D_t, 2048> _point_list;

class SmoothLineChart_Test : public SmoothLineChart
{
    float data_x = 0.0;

    void onReadInput() override
    {
        _point_list.put({static_cast<int>(data_x * 100), static_cast<int>(std::sin(data_x) * 100)});
        data_x += 0.01;
    }

    void onRender() override
    {
        HAL::GetCanvas()->fillScreen(TFT_WHITE);

        // Chart base
        HAL::GetCanvas()->fillRect(getOrigin().x, getOrigin().y, getSize().width, getSize().height, TFT_BLUE);

        // Points
        Vector2D_t last_p;
        int chart_x = 0;
        SmoothLineChart_Test* chat = this;
        _point_list.peekAll([&chart_x, &last_p, chat](Vector2D_t value) {
            auto cp = chat->getChartPoint(chart_x, value.y);

            if (chat->isInChart(cp.x, cp.y))
                HAL::GetCanvas()->setColor(TFT_YELLOW);
            else
                HAL::GetCanvas()->setColor(TFT_LIGHTGRAY);

            // HAL::GetCanvas()->fillCircle(cp.x, cp.y, 1);
            if (chart_x != 0)
                HAL::GetCanvas()->drawLine(last_p.x, last_p.y, cp.x, cp.y);
            last_p = cp;
            chart_x++;
        });

        // Values
        HAL::GetCanvas()->setFont(&fonts::efontCN_16);
        HAL::GetCanvas()->setTextColor(TFT_BLUE);
        HAL::GetCanvas()->setCursor(getOrigin().x, getOrigin().y - 32);
        HAL::GetCanvas()->printf("(x offset: % 5d) (x zoom: % 5d)", getOffset().x, getZoom().x);
        HAL::GetCanvas()->setCursor(getOrigin().x, getOrigin().y - 16);
        HAL::GetCanvas()->printf("(y offset: % 5d) (y zoom: %5d)", getOffset().y, getZoom().y);

        HAL::CanvasUpdate();
    }
};

void line_chart_test()
{
    _point_list.allowOverwrite(true);
    // for (float i = 0.0; i < 10; i += 0.01)
    // {
    //     _point_list.put({static_cast<int>(i * 100), static_cast<int>(std::sin(i) * 100)});
    // }

    SmoothLineChart_Test chart;
    chart.setOrigin(233, 166);
    chart.setSize(320, 240);
    chart.setConfig().readInputInterval = 1;

    // chart.getOffsetTransition().setTransitionPath(EasingPath::easeOutBack);
    // chart.getZoomTransition().setTransitionPath(EasingPath::easeOutBack);

    while (1)
    {
        auto time_count = HAL::Millis();
        while (1)
        {
            chart.update(HAL::Millis());
            if (HAL::Millis() - time_count > 2000)
                break;
        }

        chart.moveOffsetTo(100, 120);
        while (!chart.isFinish())
        {
            chart.update(HAL::Millis());
        }

        chart.moveZoomTo(100, ZoomBase);
        time_count = HAL::Millis();
        while (1)
        {
            chart.update(HAL::Millis());
            if (HAL::Millis() - time_count > 5000)
                break;
        }

        chart.moveZoomTo(400, 2000);
        while (!chart.isFinish())
        {
            chart.update(HAL::Millis());
        }

        chart.moveOffsetTo(0, 120);
        while (!chart.isFinish())
        {
            chart.update(HAL::Millis());
        }

        chart.moveZoomTo(1000, 1000);
        while (!chart.isFinish())
        {
            chart.update(HAL::Millis());
        }

        chart.moveZoomTo(100, 600);
        while (!chart.isFinish())
        {
            chart.update(HAL::Millis());
        }
    }
}
