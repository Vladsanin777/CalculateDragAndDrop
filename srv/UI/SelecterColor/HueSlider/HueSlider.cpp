#pragma once
#include "HueSlider.hpp"

namespace SelecterColor {
    HueSlider::HueSlider(int beginValue, \
        QColor * const &color, int endValue, \
        ColorArea2D * const colorArea2D, QWidget *parent)
        : QSlider{parent}, _colorArea2D{colorArea2D}, \
        _currentColor{color} {
        setRange(beginValue, endValue);
        setValue(color->hue());
        setOrientation(Qt::Vertical);
        //setInvertedAppearance(true);
        setMinimumSize(40, 100);
    }

    void HueSlider::updateGradient(void) {

        gradientImage = QImage(width(), height(), QImage::Format_RGB32);
        
        for (int y = 0; y < height(); ++y) {
            qreal hue = static_cast<qreal>(y) / (height() - 1);
            QColor color;
            color.setHsvF(hue, 1.0, 1.0);
            
            for (int x = 0; x < width(); ++x) {
                gradientImage.setPixelColor(x, y, color);
            }
        }
    }
    /*
    void HueSlider::setColorArea2D(ColorArea2D * colorArea2D) {
        _colorArea2D = colorArea2D; return;
    }
    void HueSlider::updateNode(void) {
        _colorArea2D->updateNode();
        return;
    }
    */

    void HueSlider::paintEvent(QPaintEvent *event) {
        Q_UNUSED(event);
        QPainter painter(this);
        printf("hjk");
        
        if (gradientDirty || gradientImage.size() != size()) {
            updateGradient();
            gradientDirty = false;
        }
        
        // Рисуем градиент с отступами по бокам
        painter.drawImage(4, 3, gradientImage.copy(1, 0, width() - 9, height()-6));
        
        qreal ratio = static_cast<qreal>(value()) / (maximum() - minimum());
        int yPos = (1 - ratio) * height();
        yPos = qBound(0, yPos - handleHeight/2, height() - handleHeight);
        
        // Рисуем ползунок на всю ширину виджета (включая отступы)
        QRect handleRect(1, yPos+1, width()-3, handleHeight-3);

        // 1. Рисуем основной белый контур с прозрачностью
        painter.setPen(QPen(QColor(255, 255, 255, 204), 1)); // 255*0.8=204
        painter.setBrush(Qt::NoBrush);
        painter.drawRect(handleRect);

        // 2. Рисуем дополнительный чёрный контур без углов
        painter.setPen(QPen(QColor(0, 0, 0, 51), 1)); // 0.2*255=51

        int x = handleRect.x();
        int y = handleRect.y();
        int w = handleRect.width();
        int h = handleRect.height();

        // Верхняя линия (без углов)
        painter.drawLine(x, y-1,     x + w, y-1);
        // Нижняя линия (без углов)
        painter.drawLine(x, y + h + 1, x + w, y + h + 1);
        // Левая линия (без углов)
        painter.drawLine(x-1,     y, x-1,     y + h);
        // Правая линия (без углов)
        painter.drawLine(x + w + 1, y, x + w + 1, y + h);
    }

    void HueSlider::mousePressEvent(QMouseEvent *event) {
        selectColorAt(event->pos());
    }

    void HueSlider::mouseMoveEvent(QMouseEvent *event) {
        if (event->buttons() & Qt::LeftButton) {
            selectColorAt(event->pos());
        }
    }
    void HueSlider::updateNode(void) {
        update();
        _colorArea2D->updateNode();
    }
    void HueSlider::selectColorAt(const QPoint &pos) {

        int hueInter{pos.y() * maximum() / height()};
        int hue{qBound(minimum(), maximum() - hueInter, maximum())};
        setValue(hue);
        int saturation{_currentColor->saturation()};
        int value{_currentColor->value()};

        int alpha{_currentColor->alpha()};
        
        _currentColor->setHsv(hueInter, saturation, value);
        _currentColor->setAlpha(alpha);
        
        updateNode();
        /*
        if (colorChangedCallback) {
            colorChangedCallback(baseHue, sat, val);
        }
        */
    }


    void HueSlider::resizeEvent(QResizeEvent *event) {
        QSlider::resizeEvent(event);
        gradientDirty = true;
    }
}