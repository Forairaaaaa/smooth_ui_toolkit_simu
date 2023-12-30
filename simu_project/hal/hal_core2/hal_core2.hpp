/**
 * @file HAL_Core2.hpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-11-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#ifdef ESP_PLATFORM
#include "../hal.h"
#include <M5Unified.h>


class HAL_Core2 : public HAL
{
public:
    std::string type() override { return "Core2"; }


    void init() override
    {
        M5.begin();

        // Display 
        _display = &M5.Display;

        // Canvas
        _canvas = new LGFX_SpriteFx(_display);
        _canvas->setPsram(true);
        _canvas->createSprite(_display->width(), _display->height());

        // // Some pc window pop up slower? 
        // lgfx::delay(1500);

        // this->popFatalError("啊?");
        // this->popFatalError("原神启动失败(悲)");
    }


    void canvasUpdate() override
    {
        GetCanvas()->pushSprite(0, 0);
        // _pw.update();
    }


    void loadTextFont24() override
    {
        // // https://github.com/Bodmer/TFT_eSPI/tree/master/Tools/Create_Smooth_Font/Create_font
        // // http://lvgl.100ask.net/8.1/tools/fonts-zh-source.html#id7
        // // https://r12a.github.io/app-conversion/
        // _canvas->loadFont("../rachel/apps/assets/fonts/zpix_cn_24.vlw");
        // _canvas->setTextSize(1);
    }


    void loadTextFont16() override
    {
        // _canvas->setFont(&fonts::efontCN_16);
        // _canvas->setTextSize(1);
    }


    void loadLauncherFont24() override
    {
        // // loadTextFont24();
        // _canvas->setFont(&fonts::efontCN_24);
        // _canvas->setTextSize(1);
    }


    bool getButton(GAMEPAD::GamePadButton_t button) override
    {
        M5.update();

        if (button == GAMEPAD::BTN_A)
            return M5.Touch.getCount() != 0;

        return false;
    }
};
#endif
