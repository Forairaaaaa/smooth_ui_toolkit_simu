/**
 * @file cubic_bezier.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-12-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <smooth_ui_toolkit.h>
#include "../../hal/hal.h"
#include "spdlog/spdlog.h"
#include <mooncake.h>


using namespace SMOOTH_UI_TK;


void cubic_bezier_test()
{
    spdlog::info("cubic bezier test");


    spdlog::info("{} {} {} {}", 
        EasingPath::easeInSine.getControlPoints().x, 
        EasingPath::easeInSine.getControlPoints().y, 
        EasingPath::easeInSine.getControlPoints().z, 
        EasingPath::easeInSine.getControlPoints().w
    );
    
}
