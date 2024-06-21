/**
 * @file app_template.cpp
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date <date></date>
 *
 * @copyright Copyright (c) 2024
 *
 */
#include "app_template.h"
#include "../../hal/hal.h"
#include "../../assets/assets.h"
#include "../utils/system/system.h"
#include <smooth_ui_toolkit.h>

using namespace MOONCAKE::APPS;
using namespace SYSTEM::INPUTS;
using namespace SmoothUIToolKit;

void AppTemplate::onResume() { spdlog::info("{} onResume", getAppName()); }

void AppTemplate::onRunning() {}

void AppTemplate::onDestroy() { spdlog::info("{} onDestroy", getAppName()); }
