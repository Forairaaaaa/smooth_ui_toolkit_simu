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
#include "core/math/math.h"
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

using namespace SmoothUIToolKit;

void setPixelAA(int x0, int y0, int t) { HAL::GetCanvas()->fillRectAlpha(x0, y0, 1, 1, (255 - t), TFT_RED); }

void plotLineAA(int x0, int y0, int x1, int y1)
{ /* draw a black (0) anti-aliased line on white (255) background */
    int sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1, x2;
    long dx = abs(x1 - x0), dy = abs(y1 - y0), err = dx * dx + dy * dy;
    long e2 = err == 0 ? 1 : 0xffff7fl / sqrt(err); /* multiplication factor */

    dx *= e2;
    dy *= e2;
    err = dx - dy; /* error value e_xy */
    for (;;)
    { /* pixel loop */
        setPixelAA(x0, y0, abs(err - dx + dy) >> 16);
        e2 = err;
        x2 = x0;
        if (2 * e2 >= -dx)
        { /* x step */
            if (x0 == x1)
                break;
            if (e2 + dy < 0xff0000l)
                setPixelAA(x0, y0 + sy, (e2 + dy) >> 16);
            err -= dy;
            x0 += sx;
        }
        if (2 * e2 <= dy)
        { /* y step */
            if (y0 == y1)
                break;
            if (dx - e2 < 0xff0000l)
                setPixelAA(x2 + sx, y0, (dx - e2) >> 16);
            err += dx;
            y0 += sy;
        }
    }
}

void plotQuadBezierSegAA(int x0, int y0, int x1, int y1, int x2, int y2)
{ /* draw an limited anti-aliased quadratic Bezier segment */
    int sx = x2 - x1, sy = y2 - y1;
    long xx = x0 - x1, yy = y0 - y1, xy;             /* relative values for checks */
    double dx, dy, err, ed, cur = xx * sy - yy * sx; /* curvature */

    assert(xx * sx <= 0 && yy * sy <= 0); /* sign of gradient must not change */

    if (sx * (long)sx + sy * (long)sy > xx * xx + yy * yy)
    { /* begin with longer part */
        x2 = x0;
        x0 = sx + x1;
        y2 = y0;
        y0 = sy + y1;
        cur = -cur; /* swap P0 P2 */
    }
    if (cur != 0)
    { /* no straight line */
        xx += sx;
        xx *= sx = x0 < x2 ? 1 : -1; /* x step direction */
        yy += sy;
        yy *= sy = y0 < y2 ? 1 : -1; /* y step direction */
        xy = 2 * xx * yy;
        xx *= xx;
        yy *= yy; /* differences 2nd degree */
        if (cur * sx * sy < 0)
        { /* negated curvature? */
            xx = -xx;
            yy = -yy;
            xy = -xy;
            cur = -cur;
        }
        dx = 4.0 * sy * (x1 - x0) * cur + xx - xy; /* differences 1st degree */
        dy = 4.0 * sx * (y0 - y1) * cur + yy - xy;
        xx += xx;
        yy += yy;
        err = dx + dy + xy; /* error 1st step */
        do
        {
            cur = fmin(dx + xy, -xy - dy);
            ed = fmax(dx + xy, -xy - dy); /* approximate error distance */
            ed += 2 * ed * cur * cur / (4 * ed * ed + cur * cur);
            setPixelAA(x0, y0, 255 * fabs(err - dx - dy - xy) / ed); /* plot curve */
            if (x0 == x2 || y0 == y2)
                break; /* last pixel -> curve finished */
            x1 = x0;
            cur = dx - err;
            y1 = 2 * err + dy < 0;
            if (2 * err + dx > 0)
            { /* x step */
                if (err - dy < ed)
                    setPixelAA(x0, y0 + sy, 255 * fabs(err - dy) / ed);
                x0 += sx;
                dx -= xy;
                err += dy += yy;
            }
            if (y1)
            { /* y step */
                if (cur < ed)
                    setPixelAA(x1 + sx, y0, 255 * fabs(cur) / ed);
                y0 += sy;
                dy -= xy;
                err += dx += xx;
            }
        } while (dy < dx); /* gradient negates -> close curves */
    }
    plotLineAA(x0, y0, x2, y2); /* plot remaining needle to end */
}

void plotQuadRationalBezierSegAA(int x0, int y0, int x1, int y1, int x2, int y2, float w)
{                                   /* draw an anti-aliased rational quadratic Bezier segment, squared weight */
    int sx = x2 - x1, sy = y2 - y1; /* relative values for checks */
    double dx = x0 - x2, dy = y0 - y2, xx = x0 - x1, yy = y0 - y1;
    double xy = xx * sy + yy * sx, cur = xx * sy - yy * sx, err, ed; /* curvature */
    bool f;

    assert(xx * sx <= 0.0 && yy * sy <= 0.0); /* sign of gradient must not change */

    if (cur != 0.0 && w > 0.0)
    { /* no straight line */
        if (sx * (long)sx + sy * (long)sy > xx * xx + yy * yy)
        { /* begin with longer part */
            x2 = x0;
            x0 -= dx;
            y2 = y0;
            y0 -= dy;
            cur = -cur; /* swap P0 P2 */
        }
        xx = 2.0 * (4.0 * w * sx * xx + dx * dx); /* differences 2nd degree */
        yy = 2.0 * (4.0 * w * sy * yy + dy * dy);
        sx = x0 < x2 ? 1 : -1; /* x step direction */
        sy = y0 < y2 ? 1 : -1; /* y step direction */
        xy = -2.0 * sx * sy * (2.0 * w * xy + dx * dy);

        if (cur * sx * sy < 0)
        { /* negated curvature? */
            xx = -xx;
            yy = -yy;
            cur = -cur;
            xy = -xy;
        }
        dx = 4.0 * w * (x1 - x0) * sy * cur + xx / 2.0 + xy; /* differences 1st degree */
        dy = 4.0 * w * (y0 - y1) * sx * cur + yy / 2.0 + xy;

        if (w < 0.5 && dy > dx)
        { /* flat ellipse, algorithm fails */
            cur = (w + 1.0) / 2.0;
            w = sqrt(w);
            xy = 1.0 / (w + 1.0);
            sx = floor((x0 + 2.0 * w * x1 + x2) * xy / 2.0 + 0.5); /* subdivide curve in half  */
            sy = floor((y0 + 2.0 * w * y1 + y2) * xy / 2.0 + 0.5);
            dx = floor((w * x1 + x0) * xy + 0.5);
            dy = floor((y1 * w + y0) * xy + 0.5);
            plotQuadRationalBezierSegAA(x0, y0, dx, dy, sx, sy, cur); /* plot apart */
            dx = floor((w * x1 + x2) * xy + 0.5);
            dy = floor((y1 * w + y2) * xy + 0.5);
            return plotQuadRationalBezierSegAA(sx, sy, dx, dy, x2, y2, cur);
        }
        err = dx + dy - xy; /* error 1st step */
        do
        { /* pixel loop */
            cur = fmin(dx - xy, xy - dy);
            ed = fmax(dx - xy, xy - dy);
            ed += 2 * ed * cur * cur / (4. * ed * ed + cur * cur); /* approximate error distance */
            x1 = 255 * fabs(err - dx - dy + xy) / ed;              /* get blend value by pixel error */
            if (x1 < 256)
                setPixelAA(x0, y0, x1); /* plot curve */
            if ((f = 2 * err + dy < 0))
            { /* y step */
                if (y0 == y2)
                    return; /* last pixel -> curve finished */
                if (dx - err < ed)
                    setPixelAA(x0 + sx, y0, 255 * fabs(dx - err) / ed);
            }
            if (2 * err + dx > 0)
            { /* x step */
                if (x0 == x2)
                    return; /* last pixel -> curve finished */
                if (err - dy < ed)
                    setPixelAA(x0, y0 + sy, 255 * fabs(err - dy) / ed);
                x0 += sx;
                dx += xy;
                err += dy += yy;
            }
            if (f)
            {
                y0 += sy;
                dy += xy;
                err += dx += xx;
            }              /* y step */
        } while (dy < dx); /* gradient negates -> algorithm fails */
    }
    plotLineAA(x0, y0, x2, y2); /* plot remaining needle to end */
}

void plotCubicBezierSegAA(int x0, int y0, float x1, float y1, float x2, float y2, int x3, int y3)
{ /* plot limited anti-aliased cubic Bezier segment */
    int f, fx, fy, leg = 1;
    int sx = x0 < x3 ? 1 : -1, sy = y0 < y3 ? 1 : -1; /* step direction */
    float xc = -fabs(x0 + x1 - x2 - x3), xa = xc - 4 * sx * (x1 - x2), xb = sx * (x0 - x1 - x2 + x3);
    float yc = -fabs(y0 + y1 - y2 - y3), ya = yc - 4 * sy * (y1 - y2), yb = sy * (y0 - y1 - y2 + y3);
    double ab, ac, bc, ba, xx, xy, yy, dx, dy, ex, px, py, ed, ip, EP = 0.01;

    /* check for curve restrains */
    /* slope P0-P1 == P2-P3     and  (P0-P3 == P1-P2      or  no slope change) */
    assert((x1 - x0) * (x2 - x3) < EP && ((x3 - x0) * (x1 - x2) < EP || xb * xb < xa * xc + EP));
    assert((y1 - y0) * (y2 - y3) < EP && ((y3 - y0) * (y1 - y2) < EP || yb * yb < ya * yc + EP));

    if (xa == 0 && ya == 0)
    { /* quadratic Bezier */
        sx = floor((3 * x1 - x0 + 1) / 2);
        sy = floor((3 * y1 - y0 + 1) / 2); /* new midpoint */
        return plotQuadBezierSegAA(x0, y0, sx, sy, x3, y3);
    }
    x1 = (x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0) + 1; /* line lengths */
    x2 = (x2 - x3) * (x2 - x3) + (y2 - y3) * (y2 - y3) + 1;
    do
    { /* loop over both ends */
        ab = xa * yb - xb * ya;
        ac = xa * yc - xc * ya;
        bc = xb * yc - xc * yb;
        ip = 4 * ab * bc - ac * ac;             /* self intersection loop at all? */
        ex = ab * (ab + ac - 3 * bc) + ac * ac; /* P0 part of self-intersection loop? */
        f = ex > 0 ? 1 : sqrt(1 + 1024 / x1);   /* calculate resolution */
        ab *= f;
        ac *= f;
        bc *= f;
        ex *= f * f; /* increase resolution */
        xy = 9 * (ab + ac + bc) / 8;
        ba = 8 * (xa - ya); /* init differences of 1st degree */
        dx = 27 * (8 * ab * (yb * yb - ya * yc) + ex * (ya + 2 * yb + yc)) / 64 - ya * ya * (xy - ya);
        dy = 27 * (8 * ab * (xb * xb - xa * xc) - ex * (xa + 2 * xb + xc)) / 64 - xa * xa * (xy + xa);
        /* init differences of 2nd degree */
        xx = 3 * (3 * ab * (3 * yb * yb - ya * ya - 2 * ya * yc) - ya * (3 * ac * (ya + yb) + ya * ba)) / 4;
        yy = 3 * (3 * ab * (3 * xb * xb - xa * xa - 2 * xa * xc) - xa * (3 * ac * (xa + xb) + xa * ba)) / 4;
        xy = xa * ya * (6 * ab + 6 * ac - 3 * bc + ba);
        ac = ya * ya;
        ba = xa * xa;
        xy = 3 * (xy + 9 * f * (ba * yb * yc - xb * xc * ac) - 18 * xb * yb * ab) / 8;

        if (ex < 0)
        { /* negate values if inside self-intersection loop */
            dx = -dx;
            dy = -dy;
            xx = -xx;
            yy = -yy;
            xy = -xy;
            ac = -ac;
            ba = -ba;
        } /* init differences of 3rd degree */
        ab = 6 * ya * ac;
        ac = -6 * xa * ac;
        bc = 6 * ya * ba;
        ba = -6 * xa * ba;
        dx += xy;
        ex = dx + dy;
        dy += xy; /* error of 1st step */

        for (fx = fy = f; x0 != x3 && y0 != y3;)
        {
            y1 = fmin(fabs(xy - dx), fabs(dy - xy));
            ed = fmax(fabs(xy - dx), fabs(dy - xy)); /* approximate error distance */
            ed = f * (ed + 2 * ed * y1 * y1 / (4 * ed * ed + y1 * y1));
            y1 = 255 * fabs(ex - (f - fx + 1) * dx - (f - fy + 1) * dy + f * xy) / ed;
            if (y1 < 256)
                setPixelAA(x0, y0, y1);                        /* plot curve */
            px = fabs(ex - (f - fx + 1) * dx + (fy - 1) * dy); /* pixel intensity x move */
            py = fabs(ex + (fx - 1) * dx - (f - fy + 1) * dy); /* pixel intensity y move */
            y2 = y0;
            do
            {                  /* move sub-steps of one pixel */
                if (ip >= -EP) /* intersection possible? -> check.. */
                    if (dx + xx > xy || dy + yy < xy)
                        goto exit; /* two x or y steps */
                y1 = 2 * ex + dx;  /* save value for test of y step */
                if (2 * ex + dy > 0)
                { /* x sub-step */
                    fx--;
                    ex += dx += xx;
                    dy += xy += ac;
                    yy += bc;
                    xx += ab;
                }
                else if (y1 > 0)
                    goto exit; /* tiny nearly cusp */
                if (y1 <= 0)
                { /* y sub-step */
                    fy--;
                    ex += dy += yy;
                    dx += xy += bc;
                    xx += ac;
                    yy += ba;
                }
            } while (fx > 0 && fy > 0); /* pixel complete? */
            if (2 * fy <= f)
            { /* x+ anti-aliasing pixel */
                if (py < ed)
                    setPixelAA(x0 + sx, y0, 255 * py / ed); /* plot curve */
                y0 += sy;
                fy += f; /* y step */
            }
            if (2 * fx <= f)
            { /* y+ anti-aliasing pixel */
                if (px < ed)
                    setPixelAA(x0, y2 + sy, 255 * px / ed); /* plot curve */
                x0 += sx;
                fx += f; /* x step */
            }
        }
        break; /* finish curve by line */
    exit:
        if (2 * ex < dy && 2 * fy <= f + 2)
        { /* round x+ approximation pixel */
            if (py < ed)
                setPixelAA(x0 + sx, y0, 255 * py / ed); /* plot curve */
            y0 += sy;
        }
        if (2 * ex > dx && 2 * fx <= f + 2)
        { /* round y+ approximation pixel */
            if (px < ed)
                setPixelAA(x0, y2 + sy, 255 * px / ed); /* plot curve */
            x0 += sx;
        }
        xx = x0;
        x0 = x3;
        x3 = xx;
        sx = -sx;
        xb = -xb; /* swap legs */
        yy = y0;
        y0 = y3;
        y3 = yy;
        sy = -sy;
        yb = -yb;
        x1 = x2;
    } while (leg--);            /* try other end */
    plotLineAA(x0, y0, x3, y3); /* remaining part in case of cusp or crunode */
}

void plotLineWidth(int x0, int y0, int x1, int y1, float wd)
{ /* plot an anti-aliased line of width wd */
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx - dy, e2, x2, y2; /* error value e_xy */
    float ed = dx + dy == 0 ? 1 : sqrt((float)dx * dx + (float)dy * dy);

    for (wd = (wd + 1) / 2;;)
    { /* pixel loop */
        setPixelAA(x0, y0, std::max(0.0f, 255 * (abs(err - dx + dy) / ed - wd + 1)));
        e2 = err;
        x2 = x0;
        if (2 * e2 >= -dx)
        { /* x step */
            for (e2 += dy, y2 = y0; e2 < ed * wd && (y1 != y2 || dx > dy); e2 += dx)
                setPixelAA(x0, y2 += sy, std::max(0.0f, 255 * (std::abs(e2) / ed - wd + 1)));
            if (x0 == x1)
                break;
            e2 = err;
            err -= dy;
            x0 += sx;
        }
        if (2 * e2 <= dy)
        { /* y step */
            for (e2 = dx - e2; e2 < ed * wd && (x1 != x2 || dx < dy); e2 += dy)
                setPixelAA(x2 += sx, y0, std::max(0.0f, 255 * (std::abs(e2) / ed - wd + 1)));
            if (y0 == y1)
                break;
            err += dx;
            y0 += sy;
        }
    }
}

void draw_line_test()
{
    // HAL::GetCanvas()->fillScreen(TFT_WHITE);

    // // plotLineWidth(100, 100, 200, 200, 1);
    // // plotLineWidth(100, 100, 200, 200, 5);

    // // plotLineWidthFixedPoint(100, 100, 600, 200, 5, _plot_line_point);

    // // SmoothUIToolKit::DrawLineAA(100, 100, 400, 500, _plot_line_point);

    // // SmoothUIToolKit::DrawLineAAWidth(300, 400, 1000, 200, 10, _plot_line_point);
    // // SmoothUIToolKit::DrawLineAAWidth(500, 777, 22, 55, 3, _plot_line_point);

    // SmoothUIToolKit::DrawLineAA(100, 100, 101, 105, _plot_line_point);

    // HAL::GetCanvas()->drawLine(100, 100 + 200, 6000, 200 + 200, TFT_BLACK);

    // for (float i = 0.0; i < 5.0; i += 0.01)
    // {
    //     SmoothUIToolKit::DrawLineAA((i - 0.01) * 500, std::sin(i - 0.01) * 500, i * 500, std::sin(i) * 500,
    //     _plot_line_point);
    //     // SmoothUIToolKit::DrawLineAAWidth(
    //     //     (i - 0.01) * 500, std::sin(i - 0.01) * 500, i * 500, std::sin(i) * 500, 5, _plot_line_point);
    // }

    // HAL::CanvasUpdate();
    // while (1)
    // {
    // }

    bool is_pressing = false;
    Vector2D_t p0(200, 200);
    Vector2D_t p1;

    while (1)
    {
        // Input
        if (HAL::IsTouching())
        {
            p0.reset(HAL::GetTouchPoint().x, HAL::GetTouchPoint().y);
        }
        else
        {
            p1.reset(HAL::GetTouchPoint().x, HAL::GetTouchPoint().y);

            // Render
            HAL::GetCanvas()->fillScreen(TFT_WHITE);

            // HAL::GetCanvas()->drawLine(p0.x, p0.y, p1.x, p1.y, TFT_RED);
            // plotLineAA(p0.x, p0.y, p1.x, p1.y);
            // plotQuadBezierSegAA(p0.x, p0.y, p1.x, p1.y, p0.x, p1.y);
            // plotQuadRationalBezierSegAA(p0.x, p0.y, p1.x, p1.y, p0.x, p1.y, 2);
            plotLineWidth(p0.x + 50, p0.y + 50, p1.x + 50, p1.y + 50, 5);
            // DrawLineAA(p0.x,
            //            p0.y,
            //            p1.x,
            //            p1.y,
            //            [](const int& x, const int& y, const int& t)
            //            { HAL::GetCanvas()->fillRectAlpha(x, y, 1, 1, (255 - t), TFT_BLACK); });

            DrawLineAAWidth(p0.x,
                            p0.y,
                            p1.x,
                            p1.y,
                            5,
                            [](const int& x, const int& y, const int& t)
                            { HAL::GetCanvas()->fillRectAlpha(x, y, 1, 1, (255 - t), TFT_BLACK); });

            HAL::CanvasUpdate();
        }
    }
}
