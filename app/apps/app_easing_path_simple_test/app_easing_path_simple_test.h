/**
 * @file app_easing_path_simple_test.h
 * @author Forairaaaaa
 * @brief
 * @version 0.1
 * @date 2024-06-21
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <cstdint>
#include <mooncake.h>

namespace MOONCAKE
{
    namespace APPS
    {
        /**
         * @brief AppEasingPathSimpleTest
         *
         */
        class AppEasingPathSimpleTest : public APP_BASE
        {
        private:
            struct Data_t
            {
                std::uint32_t time_count = 0;
            };
            Data_t _data;

        public:
            void onResume() override;
            void onRunning() override;
            void onDestroy() override;
        };

        class AppEasingPathSimpleTest_Packer : public APP_PACKER_BASE
        {
            const char* getAppName() override { return "AppEasingPathSimpleTest"; }
            void* newApp() override { return new AppEasingPathSimpleTest; }
            void deleteApp(void* app) override { delete (AppEasingPathSimpleTest*)app; }
        };
    } // namespace APPS
} // namespace MOONCAKE
