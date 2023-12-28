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
#include "lv_anim/lv_anim.h"
#include "select_menu/select_menu.h"



/**
 * @brief Wraped select menu test 
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



/**
 * @brief Raw api test 
 * 
 */
class SelectMenuRenderCb_RawApiTest : public SMOOTH_UI_TK::SimpleMenuCallback_t
{
public:
    void renderCallback(
        const std::vector<SMOOTH_UI_TK::Item_t*>& menuItemList,
        const SMOOTH_UI_TK::RenderAttribute_t& selector,
        const SMOOTH_UI_TK::RenderAttribute_t& camera
    ) override
    {
        // Clear 
        HAL::GetCanvas()->fillScreen(TFT_BLACK);

        // Render items 
        HAL::GetCanvas()->setFont(&fonts::efontCN_24);
        HAL::GetCanvas()->setTextColor(TFT_WHITE, TFT_BLACK);
        HAL::GetCanvas()->setTextSize(1);
        HAL::GetCanvas()->setTextDatum(textdatum_t::top_left);
        for (int i = 0; i < menuItemList.size(); i++)
        {
            HAL::GetCanvas()->drawString(
                menuItemList[i]->tag.c_str(), 
                menuItemList[i]->x, 
                menuItemList[i]->y - camera.y
            );
        }

        // Render selector 
        HAL::GetCanvas()->setColor(TFT_WHITE);
        HAL::GetCanvas()->fillSmoothRoundRectInDifference(
            selector.x,
            selector.y - camera.y + (menuItemList[selector.targetItem]->height - selector.height) / 2,
            selector.width,
            selector.height,
            6
        );
    }
};


void _raw_api_test()
{
    // Create a menu 
    SMOOTH_UI_TK::Simple_Menu simple_menu(HAL::GetCanvas()->width(), HAL::GetCanvas()->height());

    // Config menu (Optional)
    simple_menu.setFirstItem(0);
    simple_menu.setMenuLoopMode(true);
    {
        auto cfg = simple_menu.getMenu()->config();
        cfg.animPath_open = LVGL::ease_out;
        cfg.animTime_open = 400;
        simple_menu.getMenu()->config(cfg);
    }
    {
        auto cfg = simple_menu.getSelector()->config();
        cfg.animPath_y = LVGL::overshoot;
        cfg.animTime_y = 200;
        simple_menu.getSelector()->config(cfg);
    }
    {
        auto cfg = simple_menu.getCamera()->config();
        cfg.animPath_y = LVGL::ease_out;
        cfg.animTime_y = 400;
        simple_menu.getCamera()->config(cfg);
    }

    // Set render callback 
    SelectMenuRenderCb_RawApiTest render_callback;
    simple_menu.setRenderCallback(&render_callback);

    // Add items 
    simple_menu.getMenu()->addItem("11", 10, 10, 30, 24, nullptr);
    simple_menu.getMenu()->addItem("2222", 50, 60, 40, 40, nullptr);
    simple_menu.getMenu()->addItem("333", 100, 20, 20, 20, nullptr);
    simple_menu.getMenu()->addItem("444", 10, 80, 50, 40, nullptr);
    simple_menu.getMenu()->addItem("?55?", 70, 220, 80, 24, nullptr);
    simple_menu.getMenu()->addItem("$&66", 150, 110, 110, 24, nullptr);
    simple_menu.getMenu()->addItem("77 啊?", 200, 90, 50, 24, nullptr);

    // Done 
    // while (1)
    // {
    //     // Keep menu update and redner callback calling 
    //     simple_menu.update(HAL::Millis());
    //     // Update buffer 
    //     HAL::CanvasUpdate();
    // }

    // Update with button navigate 
    bool menu_wait_button_released = false;
    while (1) 
    {
        // Update navigation 
        if (HAL::GetButton(GAMEPAD::BTN_UP) || HAL::GetButton(GAMEPAD::BTN_LEFT))
        {
            if (!menu_wait_button_released)
            {
                simple_menu.goLast();
                menu_wait_button_released = true;
            }
        }
        else if (HAL::GetButton(GAMEPAD::BTN_DOWN) || HAL::GetButton(GAMEPAD::BTN_RIGHT))
        {
            if (!menu_wait_button_released)
            {
                simple_menu.goNext();
                menu_wait_button_released = true;
            }
        }

        // If select 
        else if (HAL::GetButton(GAMEPAD::BTN_A) || HAL::GetButton(GAMEPAD::BTN_LEFT_STICK))
        {
            simple_menu.getSelector()->pressed();
            // Wait release 
            while (HAL::GetButton(GAMEPAD::BTN_A) || HAL::GetButton(GAMEPAD::BTN_LEFT_STICK))
            {
                simple_menu.update(HAL::Millis());
                HAL::CanvasUpdate();
            }
            simple_menu.getSelector()->released();

            // Wait anim finish 
            while (!simple_menu.getSelector()->isAnimFinished())
            {
                simple_menu.update(HAL::Millis());
                HAL::CanvasUpdate();
            }

            auto selected_index = simple_menu.getSelector()->getTargetItem();
            spdlog::info("select item: {} {}", selected_index, simple_menu.getMenu()->getItemList()[selected_index]->tag);
        }

        // Unlock if no button is pressing 
        else
            menu_wait_button_released = false;


        // Keep menu update and redner callback calling 
        simple_menu.update(HAL::Millis());
        // Update buffer 
        HAL::CanvasUpdate();
    }
}



void simple_menu_test()
{
    spdlog::info("simple menu test");

    // _wraped_select_menu_test();
    _raw_api_test();
}
