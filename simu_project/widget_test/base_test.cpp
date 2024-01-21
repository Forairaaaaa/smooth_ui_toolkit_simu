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
#include "../hal/hal.h"
#include "lgfx/v1/misc/enum.hpp"
#include "spdlog/spdlog.h"
#include <mooncake.h>
#include <smooth_ui_toolkit.h>

using namespace SmoothUIToolKit;
using namespace SmoothUIToolKit::Widgets;

class WidgetTest_1 : public WidgetBase
{
public:
    WidgetTest_1(WidgetBase* parent = nullptr) { setParent(parent); }
    ~WidgetTest_1() { spdlog::info("1 bye"); }
    void onUpdate(const TimeSize_t& currentTime) override { spdlog::info("1 update"); }
    void onRender() override { spdlog::info("1 render"); }
};

class WidgetTest_2 : public WidgetBase
{
public:
    WidgetTest_2(WidgetBase* parent = nullptr) { setParent(parent); }
    ~WidgetTest_2() { spdlog::info("2 bye"); }
    void onUpdate(const TimeSize_t& currentTime) override { spdlog::info("2 update"); }
    void onRender() override { spdlog::info("2 render"); }
};

class WidgetTest_3 : public WidgetBase
{
public:
    WidgetTest_3(WidgetBase* parent = nullptr) { setParent(parent); }
    ~WidgetTest_3() { spdlog::info("3 bye"); }
    void onUpdate(const TimeSize_t& currentTime) override { spdlog::info("3 update"); }
    void onRender() override { spdlog::info("3 render"); }
};

void smooth_widget_base_test()
{
    // // 1, 2, 3 bye
    // {
    //     WidgetBase root;
    //     auto w_1 = new WidgetTest_1(&root);
    //     auto w_2 = new WidgetTest_2(&root);
    //     auto w_3 = new WidgetTest_3(&root);
    // }

    // // 1, 2, 3 bye
    // {
    //     WidgetBase root;
    //     auto w_1 = new WidgetTest_1(&root);
    //     auto w_2 = new WidgetTest_2(w_1);
    //     auto w_3 = new WidgetTest_3(w_2);
    // }

    {
        WidgetBase root;
        auto w_1 = new WidgetTest_1(&root);
        auto w_2 = new WidgetTest_2(&root);
        auto w_3 = new WidgetTest_3(&root);

        root.update(HAL::Millis());
        spdlog::info("------");

        root.setVisible(false);
        root.update(HAL::Millis());
        spdlog::info("------");

        root.setEnable(false);
        root.update(HAL::Millis());
        spdlog::info("------");
    }

    while (1)
    {
    }
}
