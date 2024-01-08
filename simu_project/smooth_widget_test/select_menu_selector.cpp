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


void select_menu_smooth_selector_test()
{
    SmoothSelector menu;

    menu.addOption({{12, 33, 4555, 6}, nullptr});
}
