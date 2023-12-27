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
#include "hal/hal_rachel/hal_rachel.h"
#endif
#include "smooth_ui_toolkit_test/smooth_ui_toolkit_test.h"


void SIMU_PROJECT::Setup()
{
    spdlog::info("simu project setup");

    // HAL injection 
    #ifndef ESP_PLATFORM
    HAL::Inject(new HAL_Simulator(240, 240));
    #else
    HAL::Inject(new HAL_Rachel);
    #endif
}


#include <random>
std::random_device rd;
std::mt19937 gen(rd());
int Game_random(int low, int high) 
{
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}


void SIMU_PROJECT::Loop()
{
    smooth_ui_toolkit_test();
}


void SIMU_PROJECT::Destroy()
{
    // Free 
    HAL::Destroy();
    spdlog::warn("simu project destroy");
}

