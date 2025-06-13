#pragma once
#include <functional>
#include "UI.hpp"

namespace Setting {
    inline SettingGrid::SettingGrid( \
        Window::Window * window, QWidget *parent) noexcept : \
        QGridLayout{parent} {
        addWidget(new Label::Label{"Bits long arifmetic", \
            fontSizeSettingLabel}, 0, 0, 1, 6);
        LineEdit::LineEdit * lineEditLongArifmetic \
            {new LineEdit::LineEdit{window}};
        window->setLineEditLongArifmetic( \
            lineEditLongArifmetic);
        addWidget(lineEditLongArifmetic, 1, 0, 1, 4);
        addWidget(new Button::ButtonBase{"Apply", window, \
            new std::function<void(QPushButton *)> \
                {std::bind(&LogicButton::applyLongArifmetic, \
                    window, std::placeholders::_1)}, \
                    fontSizeButtonSetting}, 1, 4, 1, 2);
        addWidget(new Label::Label{"Changed histori", \
            fontSizeSettingLabel}, 2, 0, 1, 6);
        addWidget(new Button::ButtonBase{"Global histori", window, \
            new std::function<void(QPushButton *)> \
                {std::bind(&LogicButton::changeHistoriVisible, \
                    window, std::placeholders::_1)}, \
                    fontSizeButtonSetting}, 3, 0, 1, 6);

        addWidget(new Label::Label{"Changed There", \
            fontSizeSettingLabel}, 4, 0, 1, 6);

        addWidget(new Button::ButtonTheme{window, LIGHT}, 5, 0, 1, 3);
        addWidget(new Button::ButtonTheme{window, DARK}, 5, 3, 1, 3);

        addWidget(new Button::ButtonTheme{window, LIGHT_RED}, 6, 0, 1, 2);
        addWidget(new Button::ButtonTheme{window, LIGHT_ORANGE}, 6, 2, 1, 2);
        addWidget(new Button::ButtonTheme{window, LIGHT_YELLO}, 6, 4, 1, 2);

        addWidget(new Button::ButtonTheme{window, DARK_RED}, 7, 0, 1, 2);
        addWidget(new Button::ButtonTheme{window, DARK_ORANGE}, 7, 2, 1, 2);
        addWidget(new Button::ButtonTheme{window, DARK_YELLO}, 7, 4, 1, 2);

        addWidget(new Button::ButtonTheme{window, LIGHT_GREEN}, 8, 0, 1, 2);
        addWidget(new Button::ButtonTheme{window, LIGHT_BLUE}, 8, 2, 1, 2);
        addWidget(new Button::ButtonTheme{window, LIGHT_PURPLE}, 8, 4, 1, 2);

        addWidget(new Button::ButtonTheme{window, DARK_GREEN}, 9, 0, 1, 2);
        addWidget(new Button::ButtonTheme{window, DARK_BLUE}, 9, 2, 1, 2);
        addWidget(new Button::ButtonTheme{window, DARK_PURPLE}, 9, 4, 1, 2);

        addWidget(new QCheckBox{"Custom theme"}, 10, 0, 1, 6);

        addLayout(new RGB::Gradient{window}, 11, 0, 1, 6);

    }
    inline SettingWidget::SettingWidget( \
        Window::Window * window, QWidget * parent) noexcept : \
        QWidget{parent} {
        setLayout(new SettingGrid{window, this});
        //setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    }
    inline SettingArea::SettingArea( \
        Window::Window * window, QWidget * parent \
    ) noexcept : QScrollArea{parent} {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setWidgetResizable(true);
        setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        setWidget(new SettingWidget{window, this});
    }
    inline void SettingArea::setWidget(QWidget *widget \
    ) noexcept {
        QScrollArea::setWidget(widget);
        if (widget) {
            widget->installEventFilter(this);
            updateMinimumWidth();
        }
    }

    inline bool SettingArea::eventFilter( \
        QObject *obj, QEvent *event \
    ) noexcept {
        if (obj == widget() && event->type() == \
            QEvent::Resize) {
            updateMinimumWidth();
        }
        return QScrollArea::eventFilter(obj, event);
    }

    inline void SettingArea::resizeEvent( \
        QResizeEvent *event) noexcept {
        QScrollArea::resizeEvent(event);
        updateMinimumWidth();
    }

    void SettingArea::updateMinimumWidth(void \
    ) noexcept {
        if (!widget()) return;

        // Рассчитываем ширину контента
        int width = widget()->sizeHint().width();
        
        // Добавляем границы области прокрутки
        width += 2 * frameWidth();
        
        // Учитываем вертикальный скроллбар (если виден)
        if (verticalScrollBar() && verticalScrollBar()->isVisible()) {
            width += verticalScrollBar()->width();
        }
        setMinimumWidth(width);
    }
    inline SettingDock::SettingDock( \
        Window::Window * window) noexcept : \
        QDockWidget{"Settings", window}, \
        _rgb0{window->getRGB0()}, \
        _rgb1{window->getRGB1()} {
        puts("SettingDock");
        setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
        setFeatures( QDockWidget::DockWidgetMovable | \
            QDockWidget::DockWidgetFloatable);
        setWidget(new SettingArea{window, this});
        setVisible(false);
    }
    inline void SettingDock::paintEvent(QPaintEvent *event) {
        Q_UNUSED(event);

        QPainter painter(this);
            
        // Создаем радиальный градиент
        QRadialGradient gradient(rect().center(), 
                                qMax(width(), height()) / 2);
        if (isFloating()) {
            
            gradient.setColorAt(0, QColor( \
                _rgb0.red(), _rgb0.green(), _rgb0.blue()));
            gradient.setColorAt(1, QColor( \
                _rgb1.red(), _rgb1.green(), _rgb1.blue()));
            
        } else {
            gradient.setColorAt(0, QColor( \
                0, 0, 0, 50));
            gradient.setColorAt(1, QColor( \
                0, 0, 0, 150));
        }
        // Заливаем фон градиентом
        painter.fillRect(rect(), gradient);
        // Важно: вызываем родительский paintEvent для отрисовки дочерних виджетов
        QDockWidget::paintEvent(event);
    }
}