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
#include <vector>


using namespace SmoothUIToolKit;


void smooth_drag_simple_test()
{
    int x_offset = HAL::GetCanvas()->width() / 2;
    int y_offset = HAL::GetCanvas()->height() / 2;



    SmoothDrag sd;

    sd.setDuration(200);
    // sd.setDuration(400);
    // sd.setDuration(800);

    // sd.setTransitionPath(EasingPath::easeOutBack);
    sd.setResetTransitionPath(EasingPath::easeOutBack);

    sd.setUpdateCallback([](SmoothDrag* smoothDrag) {
        spdlog::info("offset: ({}, {})", smoothDrag->getOffset().x, smoothDrag->getOffset().y);
    });


    // sd.setDragConfig().lockXOffset = true;
    // sd.setDragConfig().lockYOffset = true;
    // sd.setDragConfig().autoReset = true;

    // sd.setDragConfig().allowDraggingOutOfLimit = false;
    

    sd.setDragConfig().offsetLimit = true;
    sd.setDragConfig().xOffsetLimit.y = HAL::GetCanvas()->width() / 4;
    sd.setDragConfig().xOffsetLimit.x = -sd.getDragConfig().xOffsetLimit.y;
    sd.setDragConfig().yOffsetLimit.y = HAL::GetCanvas()->height() / 4;
    sd.setDragConfig().yOffsetLimit.x = -sd.getDragConfig().yOffsetLimit.y;



    bool is_touching = false;
    while (1)
    {
        HAL::GetCanvas()->fillScreen(TFT_WHITE);

        // Render limit 
        if (sd.getDragConfig().offsetLimit)
            HAL::GetCanvas()->fillRect(
                sd.getDragConfig().xOffsetLimit.x + x_offset, 
                sd.getDragConfig().yOffsetLimit.x + y_offset, 
                GetRange(sd.getDragConfig().xOffsetLimit), 
                GetRange(sd.getDragConfig().yOffsetLimit), 
                TFT_LIGHTGRAY
            );


        // Input 
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


        // Update 
        sd.update(HAL::Millis());
        // spdlog::info("offset: ({}, {})", sd.getOffset().x, sd.getOffset().y);
        

        // Render 
        HAL::GetCanvas()->fillSmoothCircle(sd.getOffset().x + x_offset, sd.getOffset().y + y_offset, 24, TFT_BLACK);

        HAL::CanvasUpdate();
    }

}



struct ContentPoint_t
{
    Vector2D_t p;
    int radius = 0;
    int color = 0;
};

#include <random>
static std::random_device rd;
static std::mt19937 gen(rd());
static int _random(int low, int high) 
{
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}

void smooth_drag_content_test()
{
    int content_w = HAL::GetCanvas()->width()  * 4;
    int content_h = HAL::GetCanvas()->height() * 4;
    int x_offset = 0;
    int y_offset = 0;
    int cp_num = 4096;
    int max_raduis = 24;

    // Generate random 
    std::vector<ContentPoint_t> content_p_list;
    for (int i = 0; i < cp_num; i++)
    {
        ContentPoint_t new_cp;
        new_cp.p.x = _random(0, content_w);
        new_cp.p.y = _random(0, content_h);
        new_cp.color = _random(0, TFT_WHITE);
        new_cp.radius = _random(2, max_raduis);
        content_p_list.emplace_back(new_cp);
    }


    SmoothDrag sd;

    sd.setDuration(200);
    // sd.setDuration(400);
    // sd.setDuration(800);

    sd.setTransitionPath(EasingPath::easeOutBounce);

    sd.setUpdateCallback([](SmoothDrag* smoothDrag) {
        spdlog::info("offset: ({}, {})", smoothDrag->getOffset().x, smoothDrag->getOffset().y);
    });

    auto cfg = sd.getDragConfig();
    // cfg.lockXOffset = true;
    cfg.lockYOffset = true;
    // cfg.autoReset = true;

    cfg.offsetLimit = true;
    cfg.xOffsetLimit.x = -(content_w - HAL::GetCanvas()->width());
    cfg.yOffsetLimit.x = -(content_h - HAL::GetCanvas()->height());

    sd.setDragConfig(cfg);

    
    bool is_touching = false;
    std::uint32_t current_time = 0;
    std::uint32_t render_time = 0;
    std::uint32_t time_count = 0;
    while (1)
    {
        HAL::GetCanvas()->fillScreen(TFT_WHITE);
    

        // Input 
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


        // Update 
        // sd.update(current_time - render_time);
        sd.update(HAL::Millis());
        

        // Render 
        current_time = HAL::Millis();


        HAL::GetCanvas()->fillRect(0 + sd.getOffset().x, 0 + sd.getOffset().y, content_w, content_h, TFT_LIGHTGRAY);
        for (int i = 0; i < cp_num; i++)
        {
            int x = content_p_list[i].p.x + sd.getOffset().x;
            int y = content_p_list[i].p.y + sd.getOffset().y;
            HAL::GetCanvas()->fillRect(x, y, content_p_list[i].radius, content_p_list[i].radius, content_p_list[i].color);
        }
        

        HAL::GetCanvas()->setTextSize(2);
        HAL::GetCanvas()->setCursor(0, 0);
        // HAL::GetCanvas()->printf("transition2d num: %d", bubble_num);
        HAL::CanvasUpdate();
        render_time = HAL::Millis() - current_time;
        // spdlog::info("render time: {}", render_time);
    }
}

