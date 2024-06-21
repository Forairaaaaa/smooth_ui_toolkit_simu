/**
 * @file apps.h
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2023-11-04
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#include "app_template/app_template.h"
#include "app_easing_path_simple_test/app_easing_path_simple_test.h"
/* Header files locator (Don't remove) */

/**
 * @brief Run startup anim app
 *
 * @param mooncake
 */
inline void app_run_startup_anim(MOONCAKE::Mooncake* mooncake)
{
    /* -------------------- Install and run startup anim here ------------------- */
    // auto startup_anim = new MOONCAKE::APPS::AppStartupAnim_Packer;

    // mooncake->createAndStartApp(startup_anim);
    // while (1)
    // {
    //     mooncake->update();
    //     if (mooncake->getAppManager()->getCreatedAppNum() == 0)
    //         break;
    // }

    // delete startup_anim;
}

/**
 * @brief Install and start default startup app
 *
 * @param mooncake
 */
inline void app_install_default_startup_app(MOONCAKE::Mooncake* mooncake)
{
    // Launcher maybe
    auto default_shit = new MOONCAKE::APPS::AppEasingPathSimpleTest_Packer;
    mooncake->installApp(default_shit);
    mooncake->createAndStartApp(default_shit);
}

/**
 * @brief Install apps
 *
 * @param mooncake
 */
inline void app_install_apps(MOONCAKE::Mooncake* mooncake)
{
    mooncake->installApp(new MOONCAKE::APPS::AppTemplate_Packer);
    mooncake->installApp(new MOONCAKE::APPS::AppEasingPathSimpleTest_Packer);
    /* Install app locator (Don't remove) */
}
