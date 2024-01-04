/**
 * @file smooth_widget_test.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2024-01-04
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <mooncake.h>
#include <smooth_ui_toolkit.h>
#include "../hal/hal.h"
#include "lgfx/v1/misc/enum.hpp"
#include "spdlog/spdlog.h"


using namespace SmoothUIToolKit::SmoothWidget;



class TestWidget : public WidgetBase
{
    void onUpdate(const std::uint32_t &currentTime) override
    {
        spdlog::info("onUpdate {}", currentTime);
    }   

    void onHover() override
    {
        spdlog::info("onHover");
    }

    void onHovering(const int &x, const int &y) override
    {
        spdlog::info("onHovering {} {}", x, y);
    }

    void onHoverEnd() override
    {
        spdlog::info("onHoverEnd");
    }

    void onDrag() override
    {
        spdlog::info("onDrag");
    }

    void onDragging(const int &x, const int &y) override
    {
        spdlog::info("onDragging {} {}", x, y);
    }

    void onDrop() override
    {
        spdlog::info("onDrop");
    }
};



void smooth_widget_base_test()
{
    TestWidget w;
    w.setPostion(400, 300);
    w.setSize(360, 480);

    

    HAL::GetCanvas()->fillScreen(TFT_WHITE);
    HAL::GetCanvas()->fillRect(w.getPostion().x, w.getPostion().y, w.getSize().width, w.getSize().height, TFT_BLUE);
    HAL::CanvasUpdate();

    bool is_touching = false;
    while (1)
    {
        HAL::UpdateTouch();
        if (HAL::IsTouching())
        {
            if (!is_touching)
                is_touching = true;

            w.drag(HAL::GetTouchPoint().x, HAL::GetTouchPoint().y);
        }
        else
        {
            if (is_touching)
            {
                is_touching = false;
                w.drop();
            }

            w.hover(HAL::GetTouchPoint().x, HAL::GetTouchPoint().y);
        }


    }
    
    

}

