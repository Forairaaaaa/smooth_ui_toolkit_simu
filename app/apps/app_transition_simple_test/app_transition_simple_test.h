/**
 * @file app_transition_simple_test.h
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
         * @brief AppTransitionSimpleTest
         *
         */
        class AppTransitionSimpleTest : public APP_BASE
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

        class AppTransitionSimpleTest_Packer : public APP_PACKER_BASE
        {
            const char* getAppName() override { return "AppTransitionSimpleTest"; }
            void* newApp() override { return new AppTransitionSimpleTest; }
            void deleteApp(void* app) override { delete (AppTransitionSimpleTest*)app; }
        };
    } // namespace APPS
} // namespace MOONCAKE
