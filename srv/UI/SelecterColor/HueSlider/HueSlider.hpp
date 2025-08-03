#pragma once

#include <QSlider>
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QImage>

#include "UI/SelecterColor/ColorArea2D/ColorArea2D.hpp"

class QSlider;
class QPaintEvent;
class QMouseEvent;
class QResizeEvent;
class QImage;

namespace SelecterColor {
    class HueSlider;
}

namespace SelecterColor {
    class HueSlider : public QSlider {
    private:
        QImage gradientImage;
        bool gradientDirty = true;
        const int handleHeight = 7; // Высота белого прямоугольника-ползунка
        ColorArea2D * const _colorArea2D;
        QColor * const &_currentColor;
    public:
        explicit HueSlider(int beginValue, \
            QColor * const &color, int endValue, \
            ColorArea2D * const colorArea2D, \
            QWidget *parent = nullptr);
        
        void updateGradient(void);
        //void setColorArea2D(ColorArea2D * const &colorArea2D);
        //void updateNode(void);
    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;
    private:
        void updateNode(void);
        void selectColorAt(const QPoint &pos);
    };
}
