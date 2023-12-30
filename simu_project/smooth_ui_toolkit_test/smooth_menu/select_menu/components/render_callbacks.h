/**
 * @file render_callbacks.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-11-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <smooth_ui_toolkit.h>


namespace SYSTEM::UI
{
    class SelectMenuRenderCb_AlignLeft : public SmoothUIToolKit::SimpleMenuCallback_t
    {
    public:
        void renderCallback(
            const std::vector<SmoothUIToolKit::Item_t*>& menuItemList,
            const SmoothUIToolKit::RenderAttribute_t& selector,
            const SmoothUIToolKit::RenderAttribute_t& camera
        ) override;
    };

    class SelectMenuRenderCb_AlignCenter : public SmoothUIToolKit::SimpleMenuCallback_t
    {
    public:
        void renderCallback(
            const std::vector<SmoothUIToolKit::Item_t*>& menuItemList,
            const SmoothUIToolKit::RenderAttribute_t& selector,
            const SmoothUIToolKit::RenderAttribute_t& camera
        ) override;
    };

    class SelectMenuRenderCb_AlignRight : public SmoothUIToolKit::SimpleMenuCallback_t
    {
    public:
        void renderCallback(
            const std::vector<SmoothUIToolKit::Item_t*>& menuItemList,
            const SmoothUIToolKit::RenderAttribute_t& selector,
            const SmoothUIToolKit::RenderAttribute_t& camera
        ) override;
    };
}