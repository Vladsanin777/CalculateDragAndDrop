#pragma once
#include <QWidget>
#include <QColor>
#include <QImage>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QSlider>


class QColor;
class QImage;
class QWidget;
class QPaintEvent;
class QMouseEvent;
class QResizeEvent;
class QSlider;

namespace SelecterColor {
    class ColorPicker;
}


namespace SelecterColor {
    class AlphaSlider;
}

namespace SelecterColor {
    class AlphaSlider : public QSlider {
    private:
        QColor baseColor = Qt::red;  // Базовый цвет (без альфа)
        QImage backgroundImage;      // Кэшированное фоновое изображение
        QImage checkerPattern;       // Шахматный паттерн
        //bool backgroundDirty = true;
        const int checkerSize = 5;   // Размер клетки шахматного паттерна
        const int handleHeight = 7; // Высота белого прямоугольника-ползунка
        ColorPicker * const _colorPicker;
        QColor * const &_currentColor;
    public:
        explicit AlphaSlider(int beginValue, \
            QColor * const &color, int endValue, \
            ColorPicker * const colorPicker, \
            QWidget *parent = nullptr);
        //void setColorPicker(QWidget * const &colorPicker);
        //void setBaseColor(const QColor &color);
        void updateBackground();
        void updateNode(void);
        //void updateNode(void);
    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;
    private:
        void drawCheckerPattern();
        void selectColorAt(const QPoint &pos);
    };
}