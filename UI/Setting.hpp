#pragma once
#include "UI"

namespace Setting {
    inline SettingGrid::SettingGrid( \
        Window::Window * window) noexcept : \
        QGridLayout{} {
        addWidget(new QLabel{"Bits long arifmetic"}, 0, 0, 1, 6);
        addWidget(new LineEdit::LineEdit{window}, 1, 0, 1, 4);
        addWidget(new Button::ButtonBase{"Apply", window, }, 1, 4, 1, 2);
    }
    inline SettingWidget::SettingWidget( \
        Window::Window * window) noexcept : \
        Qwidget{} {
        setLuyout(SettingGrid(Window));
    }
    inline SettingDock::SettingDock( \
        Window::Window * window) noexcept : \
        QDockWidget{"Settings", window} {
        setWidget(SettingWidget(window));
    }
}