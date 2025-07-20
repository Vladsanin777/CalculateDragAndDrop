#pragma once

#include <QSlider>
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QImage>

#include "../ColorArea2D/ColorArea2D.cpp"

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
        ColorArea2D * _colorArea2D{nullptr};
    public:
        explicit HueSlider(int beginValue, \
            int defaultValue, int endValue, \
            QWidget *parent = nullptr);
        
        void updateGradient(void);
        void setHueSlider(HueSlider * hueSlider);
        void updateNode(void);
    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;
    };
}