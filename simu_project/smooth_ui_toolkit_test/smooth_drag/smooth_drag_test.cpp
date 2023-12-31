/**
 * @file smooth_drag_test.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-12-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <smooth_ui_toolkit.h>
#include "../../hal/hal.h"
#include "core/easing_path/easing_path.h"
#include "lgfx/v1/misc/enum.hpp"
#include "spdlog/spdlog.h"
#include <mooncake.h>


using namespace SmoothUIToolKit;


void smooth_drag_simple_test()
{
    SmoothDrag sd;

    sd.setDuration(400);
    // sd.setDuration(800);

    // sd.setTransitionPath(EasingPath::easeOutBack);

    sd.setUpdateCallback([](SmoothDrag* smoothDrag) {
        spdlog::info("offset: ({}, {})", smoothDrag->getOffset().x, smoothDrag->getOffset().y);
    });


    bool is_touching = false;
    std::uint32_t current_time = 0;
    std::uint32_t render_time = 0;
    std::uint32_t time_count = 0;

    int x_offset = HAL::GetCanvas()->width() / 2;
    int y_offset = HAL::GetCanvas()->height() / 2;

    while (1)
    {
        HAL::GetCanvas()->fillScreen(TFT_WHITE);


        // Input 
        // if (HAL::Millis() - time_count > 0)
        if (1)
        {
            HAL::UpdateTouch();
            if (HAL::IsTouching())
            {
                is_touching = true;

                // Render tp point 
                // spdlog::info("tp: ({}, {})", HAL::GetTouchPoint().x, HAL::GetTouchPoint().y);
                HAL::GetCanvas()->fillSmoothCircle(HAL::GetTouchPoint().x, HAL::GetTouchPoint().y, 12, TFT_YELLOW);



                // On dragging 
                sd.drag(HAL::GetTouchPoint().x, HAL::GetTouchPoint().y);
            }

            // If just released 
            else if (is_touching)
            {
                is_touching = false;

                // Drop 
                sd.drop();
            }

            time_count = HAL::Millis();
        }


        // Update 
        // sd.update(current_time - render_time);
        sd.update(current_time);
        // spdlog::info("offset: ({}, {})", sd.getOffset().x, sd.getOffset().y);
        

        // Render 
        current_time = HAL::Millis(); 
        // HAL::GetCanvas()->fillScreen(TFT_WHITE);

        HAL::GetCanvas()->fillSmoothCircle(sd.getOffset().x + x_offset, sd.getOffset().y + y_offset, 24, TFT_BLACK);

        HAL::GetCanvas()->setTextSize(2);
        HAL::GetCanvas()->setCursor(0, 0);
        // HAL::GetCanvas()->printf("transition2d num: %d", bubble_num);
        HAL::CanvasUpdate();
        render_time = HAL::Millis() - current_time;
        // spdlog::info("render time: {}", render_time);
    }

}

