#pragma once
#include "ColorArea2D.hpp"
#include <iostream>

namespace SelecterColor {
    // ColorArea2D implementation (без изменений)
    ColorArea2D::ColorArea2D(QColor * const &color, \
        AlphaSlider * const &alphaSlider, QWidget *parent) 
        : QWidget(parent), _currentColor{color}, \
        _alphaSlider{alphaSlider} {
        setMinimumSize(200, 200);
    }


    /*
    void ColorArea2D::setAlphaSlider(AlphaSlider * &alphaSlider) {
        _alphaSlider = alphaSlider; return;
    }
    */
    /*
    void ColorArea2D::setBaseHue(qreal hue) {
        baseHue = qBound(0.0, hue, 1.0);
        backgroundDirty = true;
        update();

        if (colorChangedCallback) {
            colorChangedCallback(hue, saturation, value);
        }
    }

    void ColorArea2D::setSaturationValue(qreal sat, qreal val) {
        saturation = qBound(0.0, sat, 1.0);
        value = qBound(0.0, val, 1.0);
        update();
    }

    void ColorArea2D::setColorChangedCallback(std::function<void(qreal, qreal, qreal)> callback) {
        colorChangedCallback = callback;
    }

    QColor ColorArea2D::getCurrentColor() const {
        QColor color;
        color.setHsvF(baseHue, saturation, value);
        return color;
    }
    */
    void ColorArea2D::paintEvent(QPaintEvent *) {
        QPainter painter(this);
        
        if (backgroundDirty || background.size() != size()) {
            updateBackground();
            backgroundDirty = false;
        }
        
        painter.drawImage(0, 0, background);
        
        int x = _currentColor->saturationF() * width();
        int y = (1.0f - _currentColor->valueF()) * height();
        std::cout << "x: " << x << "y: " << y << std::endl;
        painter.setPen(Qt::white);
        painter.drawEllipse(QPoint(x, y), 8, 8);
        painter.setPen(Qt::black);
        painter.drawEllipse(QPoint(x, y), 9, 9);
    }

    void ColorArea2D::mousePressEvent(QMouseEvent *event) {
        selectColorAt(event->pos());
    }

    void ColorArea2D::mouseMoveEvent(QMouseEvent *event) {
        if (event->buttons() & Qt::LeftButton) {
            selectColorAt(event->pos());
        }
    }

    void ColorArea2D::resizeEvent(QResizeEvent *) {
        backgroundDirty = true;
    }

    void ColorArea2D::updateBackground() {
        background = QImage(size(), QImage::Format_RGB32);
        
        QColor color {*_currentColor};

        float hue {color.hueF()};
        for (int y = 0; y < height(); ++y) {
            float val = 1.0f - static_cast<float>(y) / height();
            for (int x = 0; x < width(); ++x) {
                float sat = static_cast<float>(x) / width();

                color.setHsvF(hue, sat, val);
                background.setPixelColor(x, y, color);
            }
        }
    }

    void ColorArea2D::selectColorAt(const QPoint &pos) {
        float hue = _currentColor->hueF();
        float sat = qBound(0.0, static_cast<float>(pos.x()) / width(), 1.0);
        float val = qBound(0.0, 1.0 - static_cast<float>(pos.y()) / height(), 1.0);
        
        _currentColor->setHsvF(hue, sat, val);
        update();
        
        /*
        if (colorChangedCallback) {
            colorChangedCallback(baseHue, sat, val);
        }
        */
    }

    void ColorArea2D::updateNode(void) {
        _alphaSlider->updateNode();
        update(); return;
    }
}