/**
 * @file anim_point_2d_test.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-12-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <smooth_ui_toolkit.h>
#include "../../hal/hal.h"
#include "lgfx/v1/misc/enum.hpp"
#include "spdlog/spdlog.h"
#include <mooncake.h>
#include <vector>


using namespace SMOOTH_UI_TK;


void anim_point_2d_test()
{
    spdlog::info("anim point 2d test");

    // Create 
    auto anim_point = AnimPoint2D();

    // Move without anim 
    anim_point.moveToWithNoAnim(20, 20);


    std::vector<Point2D_Int_t> point_list = {
        Point2D_Int_t(HAL::GetCanvas()->width() / 2, HAL::GetCanvas()->height() / 2),
        Point2D_Int_t(HAL::GetCanvas()->width() / 5, HAL::GetCanvas()->height() / 5),
        Point2D_Int_t(HAL::GetCanvas()->width() / 3, HAL::GetCanvas()->height() / 4),
        Point2D_Int_t(HAL::GetCanvas()->width() / 5 * 4, HAL::GetCanvas()->height() / 4 * 3),
        Point2D_Int_t(HAL::GetCanvas()->width() / 10 * 8, HAL::GetCanvas()->height() / 3),
    };

    for (auto i : point_list)
    {
        // Move 
        anim_point.moveTo(i);

        auto time_count = HAL::Millis();
        while (HAL::Millis() - time_count < 1000)
        {
            // Update 
            anim_point.update(HAL::Millis());

            HAL::GetCanvas()->fillScreen(TFT_BLACK);
            HAL::GetCanvas()->fillSmoothCircle(anim_point.x(), anim_point.y(), 10, TFT_YELLOW);
            HAL::CanvasUpdate();
        }
    }

}
