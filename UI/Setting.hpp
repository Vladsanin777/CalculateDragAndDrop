#pragma once
#include <functional>
#include "UI.hpp"

namespace Setting {
    inline SettingGrid::SettingGrid( \
        Window::Window * window) noexcept : \
        QGridLayout{} {
        
        addWidget(new QLabel{"Bits long arifmetic"}, 0, 0, 1, 6);
        LineEdit::LineEdit * lineEditLongArifmetic \
            {new LineEdit::LineEdit{window}};
        window->setLineEditLongArifmetic( \
            lineEditLongArifmetic);
        addWidget(lineEditLongArifmetic, 1, 0, 1, 4);
        addWidget(new Button::ButtonBase{"Apply", window, \
            new std::function<void(QPushButton *)> \
                {std::bind(&LogicButton::applyLongArifmetic, \
                    window, std::placeholders::_1)}}, 1, 4, 1, 2);
        addWidget(new QLabel{"Changed histori"}, 2, 0, 1, 6);
        addWidget(new Button::ButtonBase{"Global histori", window, \
            new std::function<void(QPushButton *)> \
                {std::bind(&LogicButton::changeHistoriVisible, \
                    window, std::placeholders::_1)}}, 3, 0, 1, 6);
    }
    inline SettingWidget::SettingWidget( \
        Window::Window * window) noexcept : \
        QWidget{} {
        setLayout(new SettingGrid{window});
    }
    inline SettingDock::SettingDock( \
        Window::Window * window) noexcept : \
        QDockWidget{"Settings", window} {
        puts("SettingDock");
        setFeatures( QDockWidget::DockWidgetMovable | \
            QDockWidget::DockWidgetFloatable);
        setWidget(new SettingWidget{window});
        setVisible(false);
    }
}