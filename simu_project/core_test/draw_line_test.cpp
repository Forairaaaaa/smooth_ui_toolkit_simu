/**
 * @file draw_line_test.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-01-26
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
#include "utils/fpm/fixed.hpp"
#include "utils/fpm/math.hpp"
#include "utils/ring_buffer/ring_buffer.h"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <mooncake.h>
#include <smooth_ui_toolkit.h>
#include <vector>

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

    HAL::GetCanvas()->drawLine(200 + 200, 200, 600 + 200, 800, TFT_BLACK);

    HAL::CanvasUpdate();

    while (1)
    {
    }
}

// http://members.chello.at/%7Eeasyfilter/bresenham.html

void setPixelColor(int32_t x, int32_t y, int32_t c)
{
    spdlog::info("{} {} {}", x, y, c);

    lgfx::rgb888_t color;
    color.r = c;
    color.g = c;
    color.b = c;

    // color.r =  (float)255 * ((float)c / 255);

    HAL::GetCanvas()->drawPixel(x, y, color);
}

void plotLineWidth(int x0, int y0, int x1, int y1, float wd)
{
    int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx - dy, e2, x2, y2; /* error value e_xy */
    float ed = dx + dy == 0 ? 1 : sqrt((float)dx * dx + (float)dy * dy);

    for (wd = (wd + 1) / 2;;)
    { /* pixel loop */
        std::max(1, 1);
        // setPixelColor(x0, y0, max(0, 255 * (abs(err - dx + dy) / ed - wd + 1)));
        setPixelColor(x0, y0, std::max(0.0f, 255 * (std::abs(err - dx + dy) / ed - wd + 1)));
        e2 = err;
        x2 = x0;
        if (2 * e2 >= -dx)
        { /* x step */
            for (e2 += dy, y2 = y0; e2 < ed * wd && (y1 != y2 || dx > dy); e2 += dx)
                setPixelColor(x0, y2 += sy, std::max(0.0f, 255 * (std::abs(e2) / ed - wd + 1)));
            if (x0 == x1)
                break;
            e2 = err;
            err -= dy;
            x0 += sx;
        }
        if (2 * e2 <= dy)
        { /* y step */
            for (e2 = dx - e2; e2 < ed * wd && (x1 != x2 || dx < dy); e2 += dy)
                setPixelColor(x2 += sx, y0, std::max(0.0f, 255 * (std::abs(e2) / ed - wd + 1)));
            if (y0 == y1)
                break;
            err += dx;
            y0 += sy;
        }
    }
}

void plotLineWidthFixedPoint(
    int x0, int y0, int x1, int y1, int width, std::function<void(const int& x, const int& y, const int& t)> plotCallback)
{
    int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx - dy, e2, x2, y2; /* error value e_xy */
    fpm::fixed_24_8 f_ed;
    if (dy == 0)
        f_ed = fpm::fixed_24_8{dx + 1};
    else
        f_ed = dx + fpm::sqrt(fpm::fixed_24_8{dx * dx + dy * dy});

    fpm::fixed_24_8 f_t;

    for (width = (width + 1) / 2;;)
    { /* pixel loop */
        std::max(1, 1);

        // setPixelColor(x0, y0, std::max(0.0f, 255 * (std::abs(err - dx + dy) / f_ed - width + 1)));
        f_t = 255 * (fpm::fixed_24_8{std::abs(err - dx + dy)} / f_ed - width + 1);
        plotCallback(x0, y0, std::max(0, static_cast<int>(f_t)));

        e2 = err;
        x2 = x0;
        if (2 * e2 >= -dx)
        { /* x step */
            for (e2 += dy, y2 = y0; fpm::fixed_24_8{e2} < f_ed * width && (y1 != y2 || dx > dy); e2 += dx)
            {
                // setPixelColor(x0, y2 += sy, std::max(0.0f, 255 * (std::abs(e2) / f_ed - width + 1)));
                f_t = 255 * (fpm::fixed_24_8{std::abs(e2)} / f_ed - width + 1);
                plotCallback(x0, y2 += sy, std::max(0, static_cast<int>(f_t)));
            }
            if (x0 == x1)
                break;
            e2 = err;
            err -= dy;
            x0 += sx;
        }
        if (2 * e2 <= dy)
        { /* y step */
            for (e2 = dx - e2; fpm::fixed_24_8{e2} < f_ed * width && (x1 != x2 || dx < dy); e2 += dy)
            {
                // setPixelColor(x2 += sx, y0, std::max(0.0f, 255 * (std::abs(e2) / ed - width + 1)));
                f_t = 255 * (fpm::fixed_24_8{std::abs(e2)} / f_ed - width + 1);
                plotCallback(x2 += sx, y0, std::max(0, static_cast<int>(f_t)));
            }
            if (y0 == y1)
                break;
            err += dx;
            y0 += sy;
        }
    }
}

void draw_line_test()
{
    HAL::GetCanvas()->fillScreen(TFT_WHITE);

    // plotLineWidth(100, 100, 200, 200, 1);
    // plotLineWidth(100, 100, 200, 200, 5);

    plotLineWidthFixedPoint(100, 100, 6000, 200, 5, [](const int& x, const int& y, const int& t) {
        spdlog::info("{} {} {}", x, y, t);

        lgfx::rgb888_t color;
        color.r = t;
        color.g = t;
        color.b = t;

        HAL::GetCanvas()->drawPixel(x, y, color);
    });


    HAL::GetCanvas()->drawLine(100, 100 + 200, 6000, 200 + 200, TFT_BLACK);

    HAL::CanvasUpdate();

    while (1)
    {
    }
}
