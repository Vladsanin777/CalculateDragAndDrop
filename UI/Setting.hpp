#pragma once
#include "UI"

namespace Setting {
    inline SettingWidget::SettingWidget( \
        Window::Window * window) noexcept {
        
    }
    inline SettingDock::SettingDock( \
        Window::Window * window) noexcept {
        SettingDock(window);
    }
}