/**
 * @file select_menu_selector.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-01-08
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "../hal/hal.h"
#include "core/easing_path/easing_path.h"
#include "lgfx/v1/misc/enum.hpp"
#include "spdlog/spdlog.h"
#include <mooncake.h>
#include <smooth_ui_toolkit.h>

using namespace SmoothUIToolKit;
using namespace SmoothUIToolKit::SelectMenu;

class SmoothSelector_test : public SmoothSelector
{
    bool _wait_button_released = false;
    bool _is_pressing = false;

    void onGoLast() override { spdlog::info("go last"); }

    void onGoNext() override { spdlog::info("go next"); }

    void onPress() override { spdlog::info("on press"); }

    void onRelease() override { spdlog::info("on release"); }

    void onReadInput() override
    {
        // spdlog::info("on input");

        // Update navigation
        if (HAL::GetButton(GAMEPAD::BTN_UP) || HAL::GetButton(GAMEPAD::BTN_LEFT))
        {
            if (!_wait_button_released)
            {
                goLast();
                _wait_button_released = true;
            }
        }
        else if (HAL::GetButton(GAMEPAD::BTN_DOWN) || HAL::GetButton(GAMEPAD::BTN_RIGHT))
        {
            if (!_wait_button_released)
            {
                goNext();
                _wait_button_released = true;
            }
        }

        // If select
        else if (HAL::GetButton(GAMEPAD::BTN_A) || HAL::GetButton(GAMEPAD::BTN_LEFT_STICK))
        {
            if (!_wait_button_released)
            {
                // Squeeze it
                press({0, 12, 240, 52});
                _wait_button_released = true;
                _is_pressing = true;
            }
        }

        // Unlock if no button is pressing
        else
        {
            _wait_button_released = false;
            if (_is_pressing)
            {
                _is_pressing = false;
                release();
            }
        }
    }

    void onRender() override
    {
        // spdlog::info("on render");

        // Clear
        HAL::GetCanvas()->fillScreen(TFT_WHITE);

        // Redner options
        int index = 0;
        for (auto& i : getOptionList())
        {
            HAL::GetCanvas()->fillRect(i.keyframe.x, i.keyframe.y, i.keyframe.w, i.keyframe.h, TFT_BLUE);
            HAL::GetCanvas()->setTextColor(TFT_YELLOW);
            // HAL::GetCanvas()->setTextSize(4);
            HAL::GetCanvas()->setCursor(i.keyframe.x, i.keyframe.y);
            HAL::GetCanvas()->printf("%d", index);

            index++;
        }

        // Render selector
        HAL::GetCanvas()->setColor(TFT_RED);
        HAL::GetCanvas()->fillRectInDifference(
            getSelectorCurrentFrame().x, getSelectorCurrentFrame().y, getSelectorCurrentFrame().w, getSelectorCurrentFrame().h);

        // Render camera
        HAL::GetCanvas()->drawRect(
            getCameraOffset().x, getCameraOffset().y, getCameraSize().width, getCameraSize().height, TFT_GREEN);

        HAL::CanvasUpdate();
    }
};

void select_menu_smooth_selector_test()
{
    SmoothSelector_test menu;

    // menu.setConfig().moveInLoop = false;
    menu.setCameraSize(128, 66);

    menu.getSelectorPostion().setDuration(400);
    menu.getSelectorShape().setTransitionPath(EasingPath::easeOutBack);

    menu.addOption({{23, 24 + 24 * 1, 60, 10}, nullptr});
    menu.addOption({{10, 24 + 24 * 2, 10, 60}, nullptr});
    menu.addOption({{5, 24 + 24 * 3, 60, 10}, nullptr});
    menu.addOption({{30, 24 + 24 * 4, 100, 100}, nullptr});
    menu.addOption({{200, 24 + 24 * 5, 24, 50}, nullptr});
    menu.addOption({{280, 24 + 24 * 2 + 33, 32, 100}, nullptr});

    while (1)
    {
        menu.update(HAL::Millis());
    }
}
