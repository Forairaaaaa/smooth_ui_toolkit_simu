/**
 * @file select_menu.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-01-06
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

class SmoothOption_Test : public SmoothOptions
{
    bool _wait_button_released = false;

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
        }

        // Unlock if no button is pressing
        else
            _wait_button_released = false;
    }

    void onRender() override
    {
        // spdlog::info("on render");

        // Clear
        HAL::GetCanvas()->fillScreen(TFT_WHITE);

        // Redner options in keyframe order
        int i = 0;
        int matching_index = 0;
        for (const auto& keyframe : getKeyframeList())
        {
            // // Render keyframes
            // HAL::GetCanvas()->fillRect(keyframe.x, keyframe.y, keyframe.w, keyframe.h, TFT_YELLOW);

            // Render options
            getMatchingOptionIndex(i, matching_index);
            HAL::GetCanvas()->fillRect(getOptionCurrentFrame(matching_index).x, getOptionCurrentFrame(matching_index).y,
                                       getOptionCurrentFrame(matching_index).w, getOptionCurrentFrame(matching_index).h,
                                       TFT_BLUE

            );
            HAL::GetCanvas()->setCursor(getOptionCurrentFrame(matching_index).x, getOptionCurrentFrame(matching_index).y);
            HAL::GetCanvas()->setTextSize(4);
            HAL::GetCanvas()->setTextColor(TFT_BLACK);
            HAL::GetCanvas()->printf("%d", matching_index);

            i++;
        }

        HAL::CanvasUpdate();
    }
};

void select_menu_test()
{
    SmoothOption_Test menu;
    // menu.setConfig().renderInterval = 500;

    menu.setConfig().renderInterval = 0;

    menu.addOption();
    menu.setLastKeyframe({100, 15, 333, 168});
    for (int i = 0; i < 5; i++)
    {
        menu.addOption();
        menu.setLastKeyframe({100 + (100 + 32) * i, 233 + (60 + 25) * i, 123, 62});
    }
    menu.addOption();
    menu.setLastKeyframe({HAL::GetCanvas()->width() - 200, 30, 12, 12});

    // After adding the options so we have shit to config
    // menu.setPositionDuration(400);
    // menu.setAllPositionTransitionPath(EasingPath::easeOutBack);
    // menu.setShapeDuration(400);
    // menu.setAllShapeTransitionPath(EasingPath::easeOutBack);

    menu.setDuration(400);

    // Update with button navigate
    bool wait_button_released = false;
    while (1)
    {
        // Update menu
        menu.update(HAL::Millis());
    }
}
