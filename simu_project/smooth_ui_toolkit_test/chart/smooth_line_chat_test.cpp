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
#include "../../hal/hal.h"
#include "core/easing_path/easing_path.h"
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
    void onReadInput() override {}

    void onRender() override
    {
        HAL::GetCanvas()->fillScreen(TFT_WHITE);

        // Chart base
        HAL::GetCanvas()->fillRect(getOrigin().x, getOrigin().y, getSize().width, getSize().height, TFT_BLUE);

        // Points
        Vector2D_t last_p;
        int i = 0;
        SmoothLineChart_Test* chat = this;
        _point_list.peekAll([&last_p, chat](Vector2D_t value) {
            auto cp = chat->getChartPoint(value);
            if (chat->isInChart(cp.x, cp.y))
                HAL::GetCanvas()->setColor(TFT_YELLOW);
            else
                HAL::GetCanvas()->setColor(TFT_BLACK);

            HAL::GetCanvas()->fillCircle(cp.x, cp.y, 1);
            // if (i != 0)
            //     HAL::GetCanvas()->drawLine(last_p.x, last_p.y, cp.x, cp.y);
            // last_p = cp;
        });

        HAL::CanvasUpdate();
    }
};

void line_chart_test()
{
    _point_list.allowOverwrite(true);
    for (float i = 0.0; i < 10; i += 0.01)
    {
        _point_list.put({static_cast<int>(i * 100), static_cast<int>(std::sin(i) * 100)});
    }

    SmoothLineChart_Test chart;
    chart.setOrigin(233, 166);
    chart.setSize(320, 240);

    chart.getOffsetTransition().setTransitionPath(EasingPath::easeOutBack);
    chart.getZoomTransition().setTransitionPath(EasingPath::easeOutBack);

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
        while (!chart.isFinish())
        {
            chart.update(HAL::Millis());
        }

        chart.moveZoomTo(100, 2000);
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
    }
}
