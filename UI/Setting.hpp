#pragma once
#include <functional>
#include "UI.hpp"

namespace Setting {
    inline SettingGrid::SettingGrid( \
        Window::Window * window, QWidget *parent) noexcept : \
        QGridLayout{parent} {
        addWidget(new Label::LabelBasic{"Bits long arifmetic", \
            fontSizeSettingLabel}, 0, 0, 1, 6);
        LineEdit::LineEdit * lineEditLongArifmetic \
            {new LineEdit::LineEdit{window}};
        window->setLineEditLongArifmetic( \
            lineEditLongArifmetic);
        addWidget(lineEditLongArifmetic, 1, 0, 1, 4);
        addWidget(new Button::ButtonBase{"Apply", window, \
            new std::function<void(QPushButton *)> \
                {std::bind(&LogicButton::applyLongArifmetic, \
                    window, std::placeholders::_1)}}, 1, 4, 1, 2);
        addWidget(new Label::LabelBasic{"Changed histori", \
            fontSizeSettingLabel}, 2, 0, 1, 6);
        addWidget(new Button::ButtonBase{"Global histori", window, \
            new std::function<void(QPushButton *)> \
                {std::bind(&LogicButton::changeHistoriVisible, \
                    window, std::placeholders::_1)}}, 3, 0, 1, 6);

        addWidget(new Label::LabelBasic{"Changed There", \
            fontSizeSettingLabel}, 4, 0, 1, 6);

        addWidget(new Button::ButtonTheme{window, LIGHT_RED}, 5, 0, 1, 1);
        addWidget(new Button::ButtonTheme{window, LIGHT_ORANGE}, 5, 1, 1, 1);
        addWidget(new Button::ButtonTheme{window, LIGHT_YELLO}, 5, 2, 1, 1);
        addWidget(new Button::ButtonTheme{window, LIGHT_GREEN}, 5, 3, 1, 1);
        addWidget(new Button::ButtonTheme{window, LIGHT_BLUE}, 5, 4, 1, 1);
        addWidget(new Button::ButtonTheme{window, LIGHT_PURPLE}, 5, 5, 1, 1);

        addWidget(new Button::ButtonTheme{window, DARK_RED}, 6, 0, 1, 1);
        addWidget(new Button::ButtonTheme{window, DARK_ORANGE}, 6, 1, 1, 1);
        addWidget(new Button::ButtonTheme{window, DARK_YELLO}, 6, 2, 1, 1);
        addWidget(new Button::ButtonTheme{window, DARK_GREEN}, 6, 3, 1, 1);
        addWidget(new Button::ButtonTheme{window, DARK_BLUE}, 6, 4, 1, 1);
        addWidget(new Button::ButtonTheme{window, DARK_PURPLE}, 6, 5, 1, 1);

    }
    inline SettingWidget::SettingWidget( \
        Window::Window * window, QWidget * parent) noexcept : \
        QWidget{parent} {
        setLayout(new SettingGrid{window, this});
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    }
    inline SettingArea::SettingArea( \
        Window::Window * window, QWidget * parent \
    ) noexcept : QScrollArea{parent} {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setWidgetResizable(false);
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
        setFeatures( QDockWidget::DockWidgetMovable | \
            QDockWidget::DockWidgetFloatable);
        setWidget(new SettingArea{window, this});
        setVisible(false);
    }
    inline void SettingDock::paintEvent(QPaintEvent *event) {
        Q_UNUSED(event);
        if (isFloating()) {
            QPainter painter(this);
            
            // Создаем радиальный градиент
            QRadialGradient gradient(rect().center(), 
                                    qMax(width(), height()) / 2);
            
            gradient.setColorAt(0, QColor( \
                _rgb0.red(), _rgb0.green(), _rgb0.blue()));
            gradient.setColorAt(1, QColor( \
                _rgb1.red(), _rgb1.green(), _rgb1.blue()));
            
            // Заливаем фон градиентом
            painter.fillRect(rect(), gradient);
        }
        // Важно: вызываем родительский paintEvent для отрисовки дочерних виджетов
        QDockWidget::paintEvent(event);
    }
}