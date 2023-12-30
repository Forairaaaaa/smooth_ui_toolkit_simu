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


using namespace SmoothUIToolKit;


static void _simple_test()
{
    Transition t1;


    // Setup by config 
    // auto cfg = t1.getConfig();
    // ...
    // t1.setConfig(cfg);


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
        HAL::GetCanvas()->fillSmoothCircle(t1.getValue(), 200, 24, TFT_BLACK);
        HAL::CanvasUpdate();
    }
}


void transition_test()
{
    spdlog::info("transition test");

    _simple_test();
}
