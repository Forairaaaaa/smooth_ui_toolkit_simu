/**
 * @file rachel.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-11-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "simu_project.h"
#include <mooncake.h>
#include "hal/hal.h"
#ifndef ESP_PLATFORM
#include "hal/hal_simulator/hal_simulator.hpp"
#else
#include "hal/hal_core2/hal_core2.hpp"
#endif
#include "smooth_ui_toolkit_test/smooth_ui_toolkit_test.h"


void SIMU_PROJECT::Setup()
{
    spdlog::info("simu project setup");

    // HAL injection 
    #ifndef ESP_PLATFORM
    // HAL::Inject(new HAL_Simulator(640, 640));
    // HAL::Inject(new HAL_Simulator(320, 240));
    HAL::Inject(new HAL_Simulator(1280, 800));
    #else
    HAL::Inject(new HAL_Core2());
    #endif
}


// #include <random>
// std::random_device rd;
// std::mt19937 gen(rd());
// int Game_random(int low, int high) 
// {
//     std::uniform_int_distribution<> dist(low, high);
//     return dist(gen);
// }



void smooth_widget_base_test();
void select_menu_test();


void SIMU_PROJECT::Loop()
{
    // easing_path_simple_test();
    // easing_path_play_with_transition();
    
    // transition_simple_test();
    // transition_user_data_test();

    // smooth_point_simple_test();
    // smooth_point_bubble_pool_test();

    // smooth_drag_simple_test();
    // smooth_drag_content_test();


    // smooth_widget_base_test();

    select_menu_test();
}


void SIMU_PROJECT::Destroy()
{
    // Free 
    HAL::Destroy();
    spdlog::warn("simu project destroy");
}

