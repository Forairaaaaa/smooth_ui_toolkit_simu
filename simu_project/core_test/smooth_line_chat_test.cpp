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
#include "core/types/types.h"
#include "lgfx/v1/lgfx_fonts.hpp"
#include "lgfx/v1/misc/colortype.hpp"
#include "lgfx/v1/misc/enum.hpp"
#include "spdlog/spdlog.h"
#include "utils/ring_buffer/ring_buffer.h"
#include <cmath>
#include <mooncake.h>
#include <smooth_ui_toolkit.h>
#include <vector>

using namespace SmoothUIToolKit;
using namespace SmoothUIToolKit::Chart;

RingBuffer<VectorFloat2D_t, 2048> _point_list;

class SmoothLineChart_Test : public SmoothLineChart
{
    float data_x = 0.0;

    void onReadInput() override
    {
        _point_list.put({data_x * 100, std::sin(data_x) * 100});
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
        _point_list.peekAll([&chart_x, &last_p, chat](VectorFloat2D_t value) {
            auto cp = chat->getChartPoint(chart_x, value.y);
            // spdlog::info("{} {}", cp.x, cp.y);

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
        HAL::GetCanvas()->printf("(x offset: %8.3f) (x zoom: %8.3f)", getCurrentOffset().x, getCurrentZoom().x);
        HAL::GetCanvas()->setCursor(getOrigin().x, getOrigin().y - 16);
        HAL::GetCanvas()->printf("(y offset: %8.3f) (y zoom: %8.3f)", getCurrentOffset().y, getCurrentZoom().y);

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

        chart.moveZoomTo(0.1, 1);
        time_count = HAL::Millis();
        while (1)
        {
            chart.update(HAL::Millis());
            if (HAL::Millis() - time_count > 5000)
                break;
        }

        chart.moveZoomTo(0.4, 2);
        while (!chart.isFinish())
        {
            chart.update(HAL::Millis());
        }

        chart.moveOffsetTo(0, 120);
        while (!chart.isFinish())
        {
            chart.update(HAL::Millis());
        }

        chart.moveZoomTo(1, 1);
        while (!chart.isFinish())
        {
            chart.update(HAL::Millis());
        }

        chart.moveZoomTo(0.1, 0.6);
        while (!chart.isFinish())
        {
            chart.update(HAL::Millis());
        }
    }
}

class SmoothLineChart_Test2 : public SmoothLineChart_Test
{
    float data_x = 0.0;

    void onReadInput() override
    {
        _point_list.put({data_x * 100, std::sin(data_x)});
        data_x += 0.01;
    }
};

void line_chart_test2()
{
    HAL::Delay(2000);

    // _point_list.allowOverwrite(false);
    // for (float i = 0.0; i < 10; i += 0.01)
    // {
    //     _point_list.put({static_cast<int>(i * 100), static_cast<int>(std::sin(i) * 100)});
    // }

    SmoothLineChart_Test2 chart;
    chart.setOrigin(233, 166);
    chart.setSize(320, 240);
    chart.setConfig().readInputInterval = 1;

    // chart.getOffsetTransition().setTransitionPath(EasingPath::easeOutBack);
    // chart.getZoomTransition().setTransitionPath(EasingPath::easeOutBack);

    // chart.moveZoomTo(1, 120);
    // chart.moveOffsetTo(0, 1);
    // chart.moveZoomTo(1, 120);

    auto y_zoom = chart.getZoomByRange(-1, 1, chart.getConfig().size.height);
    spdlog::info("{}", y_zoom);
    auto x_zoom = chart.getZoomByRange(0, 2048, chart.getConfig().size.width);
    spdlog::info("{}", x_zoom);

    // chart.moveZoomTo(1, chart.getZoomByRange(-1, 1, chart.getConfig().size.height));

    // chart.moveOffsetTo(512, 1);
    // chart.moveXOffsetTo(512);

    // chart.moveZoomTo(x_zoom, y_zoom);

    // chart.moveYIntoRange(-2, 2);
    // chart.moveXIntoRange(0 - 512, 2048 - 512);
    // chart.moveXIntoRange(0 - 512, 2048 - 512);

    while (1)
    {
        chart.update(HAL::Millis());

        chart.moveZoomTo(1, 1);
        chart.moveOffsetTo(0, 0);
        auto time_count = HAL::Millis();
        while (1)
        {
            chart.update(HAL::Millis());
            if (HAL::Millis() - time_count > 2000)
                break;
        }

        chart.moveYIntoRange(-1, 1);
        time_count = HAL::Millis();
        while (1)
        {
            chart.update(HAL::Millis());
            if (HAL::Millis() - time_count > 2000)
                break;
        }

        chart.moveXIntoRange(0, 2048);
        time_count = HAL::Millis();
        while (1)
        {
            chart.update(HAL::Millis());
            if (HAL::Millis() - time_count > 2000)
                break;
        }

        chart.moveYIntoRange(-2, 2);
        chart.moveXIntoRange(0 - 512, 2048 + 512);
        time_count = HAL::Millis();
        while (1)
        {
            chart.update(HAL::Millis());
            if (HAL::Millis() - time_count > 2000)
                break;
        }
    }
}

// https://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm
class XiaolinWuLineAlgorithm
{
private:
    std::function<void(float x, float y, float c)> _plot;

    float _ipart(float x) { return std::floor(x); };

    float _round(float x) { return _ipart(x + 0.5); }

    float _fpart(float x) { return x - _ipart(x); }

    float _rfpart(float x) { return 1 - _fpart(x); }

public:
    inline void setPlotCallback(std::function<void(float x, float y, float c)> plot) { _plot = plot; }

    void drawLine(int32_t x0, int32_t y0, int32_t x1, int32_t y1)
    {
        bool steep = std::abs(y1 - y0) > abs(x1 - x0);

        if (steep)
        {
            std::swap(x0, y0);
            std::swap(x1, y1);
        }
        if (x0 > x1)
        {
            std::swap(x0, x1);
            std::swap(y0, y1);
        }

        float dx = x1 - x0;
        float dy = y1 - y0;
        float gradient = 0.0f;

        if (dx == 0.0f)
            gradient = 1.0f;
        else
            gradient = dy / dx;

        // handle first endpoint
        float xend = _round(x0);
        float yend = y0 + gradient * (xend - x0);
        float xgap = _rfpart(x0 + 0.5);
        float xpxl1 = xend; // this will be used in the main loop
        float ypxl1 = _ipart(yend);

        if (steep)
        {
            _plot(ypxl1, xpxl1, _rfpart(yend) * xgap);
            _plot(ypxl1 + 1, xpxl1, _fpart(yend) * xgap);
        }
        else
        {
            _plot(xpxl1, ypxl1, _rfpart(yend) * xgap);
            _plot(xpxl1, ypxl1 + 1, _fpart(yend) * xgap);
        }
        float intery = yend + gradient; // first y-intersection for the main loop

        // handle second endpoint
        xend = _round(x1);
        yend = y1 + gradient * (xend - x1);
        xgap = _fpart(x1 + 0.5);
        float xpxl2 = xend; // this will be used in the main loop
        float ypxl2 = _ipart(yend);

        if (steep)
        {
            _plot(ypxl2, xpxl2, _rfpart(yend) * xgap);
            _plot(ypxl2 + 1, xpxl2, _fpart(yend) * xgap);
        }
        else
        {
            _plot(xpxl2, ypxl2, _rfpart(yend) * xgap);
            _plot(xpxl2, ypxl2 + 1, _fpart(yend) * xgap);
        }

        // main loop
        if (steep)
        {
            for (float x = (xpxl1 + 1); x <= (xpxl2 - 1); x += 1.0f)
            {
                _plot(_ipart(intery), x, _rfpart(intery));
                _plot(_ipart(intery) + 1, x, _fpart(intery));
                intery = intery + gradient;
            }
        }
        else
        {
            for (float x = (xpxl1 + 1); x <= (xpxl2 - 1); x += 1.0f)
            {
                _plot(x, _ipart(intery), _rfpart(intery));
                _plot(x, _ipart(intery) + 1, _fpart(intery));
                intery = intery + gradient;
            }
        }
    }
};

void xiaolinwu_line_test()
{
    HAL::GetCanvas()->fillScreen(TFT_WHITE);

    XiaolinWuLineAlgorithm line;

    line.setPlotCallback([](float x, float y, float c) {
        spdlog::info("{} {} {}", x, y, c);

        lgfx::rgb888_t color;
        // color.r = 255;
        color.r = (float)255 * (1 - c);
        color.g = (float)255 * (1 - c);
        color.b = (float)255 * (1 - c);
        // spdlog::info("{}", color.r);

        HAL::GetCanvas()->drawPixel(x, y, color);
        // HAL::GetCanvas()->drawPixel(x, y, TFT_BLUE);
    });

    // line.drawLine(0, 0, 5, 5);
    // line.drawLine(0, 0, 100, 100);
    // line.drawLine(200, 200, 400, 250);
    line.drawLine(200, 200, 600, 800);

    HAL::CanvasUpdate();

    while (1)
    {
    }
}
