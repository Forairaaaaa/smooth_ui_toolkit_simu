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
    class SelectMenuRenderCb_AlignLeft : public SMOOTH_UI_TK::SimpleMenuCallback_t
    {
    public:
        void renderCallback(
            const std::vector<SMOOTH_UI_TK::Item_t*>& menuItemList,
            const SMOOTH_UI_TK::RenderAttribute_t& selector,
            const SMOOTH_UI_TK::RenderAttribute_t& camera
        ) override;
    };

    class SelectMenuRenderCb_AlignCenter : public SMOOTH_UI_TK::SimpleMenuCallback_t
    {
    public:
        void renderCallback(
            const std::vector<SMOOTH_UI_TK::Item_t*>& menuItemList,
            const SMOOTH_UI_TK::RenderAttribute_t& selector,
            const SMOOTH_UI_TK::RenderAttribute_t& camera
        ) override;
    };

    class SelectMenuRenderCb_AlignRight : public SMOOTH_UI_TK::SimpleMenuCallback_t
    {
    public:
        void renderCallback(
            const std::vector<SMOOTH_UI_TK::Item_t*>& menuItemList,
            const SMOOTH_UI_TK::RenderAttribute_t& selector,
            const SMOOTH_UI_TK::RenderAttribute_t& camera
        ) override;
    };
}