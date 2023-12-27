/**
 * @file simple_menu_test.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-12-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <smooth_ui_toolkit.h>
#include "../../hal/hal.h"
#include <mooncake.h>
#include "select_menu/select_menu.h"


/**
 * @brief Wraped select menu 
 * 
 */
void _wraped_select_menu_test()
{
    std::vector<std::string> item_list = {
        "[?????]",
        "asdasd",
        "89809fhgbhh fdf?",
        "iahs asfdh",
        "#@^%&^*&^(& 6784)",
        "13156185",
    };

    SYSTEM::UI::SelectMenu menu;
    auto selected = menu.waitResult(item_list);
    spdlog::info("selected item: {} {}", selected, item_list[selected]);
}


void simple_menu_test()
{
    spdlog::info("simple menu test");

    _wraped_select_menu_test();
    
}
