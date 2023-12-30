/**
 * @file transition_test.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-12-30
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



void transition_simple_test()
{
    Transition t1;


    // Setup 
    t1.setStartValue(0);
    t1.setEndValue(100);


    // Or setup by helper 
    t1.setConfig(HAL::GetCanvas()->width() / 6, HAL::GetCanvas()->width() / 6 * 5, 1000, EasingPath::easeOutQuad);


    // Set update callback (optional)
    t1.setUpdateCallback([](Transition* transition) {
        spdlog::info("value: {}", transition->getValue());

        // If finish, invert back 
        if (transition->isFinish())
        {
            transition->setConfig(transition->getEndValue(), transition->getStartValue());
            transition->setDelay(500);
            transition->reset();
            transition->start(HAL::Millis());
        }
    });


    // Start transtion 
    t1.start(HAL::Millis());


    while (1)
    {
        // Update transition 
        t1.update(HAL::Millis());

        HAL::GetCanvas()->fillScreen(TFT_WHITE);
        HAL::GetCanvas()->fillSmoothCircle(t1.getValue(), HAL::GetCanvas()->height() / 2, 24, TFT_BLACK);
        HAL::CanvasUpdate();
    }
}



struct UserDataTest_t
{
    int yOffset = 0;
    int radius = 0;

    UserDataTest_t(int yOffset, int radius)
    {
        this->yOffset = yOffset;
        this->radius = radius;
    }
};


static void _render_callback(Transition* transition)
{
    // Reder 
    auto user_data = static_cast<UserDataTest_t*>(transition->getUserData());
    HAL::GetCanvas()->fillSmoothCircle(transition->getValue(), user_data->yOffset, user_data->radius, TFT_BLACK);

    // If finish, invert back 
    if (transition->isFinish())
    {
        spdlog::info("finish");

        transition->setConfig(transition->getEndValue(), transition->getStartValue());
        transition->setDelay(500);
        transition->reset();
        transition->start(HAL::Millis());
    }
}


void transition_user_data_test()
{
    auto x_start = HAL::GetCanvas()->width() / 6;
    auto x_end = HAL::GetCanvas()->width() / 6 * 5;
    

    std::vector<Transition> t_list;
    t_list.emplace_back(Transition(x_start, x_end, 1000, EasingPath::easeOutSine));
    t_list.emplace_back(Transition(x_start, x_end, 1000, EasingPath::easeOutQuad));
    t_list.emplace_back(Transition(x_start, x_end, 1000, EasingPath::easeOutCubic));
    t_list.emplace_back(Transition(x_start, x_end, 1000, EasingPath::easeOutQuart));
    t_list.emplace_back(Transition(x_start, x_end, 1000, EasingPath::easeOutQuint));
    t_list.emplace_back(Transition(x_start, x_end, 1000, EasingPath::easeOutExpo));
    t_list.emplace_back(Transition(x_start, x_end, 1000, EasingPath::easeOutCirc));
    t_list.emplace_back(Transition(x_start, x_end, 1000, EasingPath::easeOutBack));
    t_list.emplace_back(Transition(x_start, x_end, 1000, EasingPath::easeOutElastic));
    t_list.emplace_back(Transition(x_start, x_end, 1000, EasingPath::easeOutBounce));


    // Set user data, callback and start 
    for (int i = 0; i < t_list.size(); i++)
    {
        int y_offset = HAL::GetCanvas()->height() / t_list.size() * i;
        y_offset += HAL::GetCanvas()->height() / t_list.size() / 2;
        int radius = HAL::GetCanvas()->height() / t_list.size() / 4;
        if (radius < 2)
            radius = 2;

        t_list[i].setUserData(new UserDataTest_t(y_offset, radius));
        t_list[i].setUpdateCallback(_render_callback);
        t_list[i].start(HAL::Millis());
    }


    while (1)
    {
        HAL::GetCanvas()->fillScreen(TFT_WHITE);
        for (auto& t : t_list)
        {
            t.update(HAL::Millis());
        }
        #ifdef ESP_PLATFORM
        HAL::RenderFpsPanel();
        #endif
        HAL::CanvasUpdate();
    }
}
