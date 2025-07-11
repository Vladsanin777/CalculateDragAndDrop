#pragma once

#include <QSlider>
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QImage>

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
    public:
        explicit HueSlider(int beginValue, \
            int endValue, int defaultValue, \
            QWidget *parent = nullptr);
        
        void updateGradient();
    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;
    };
}