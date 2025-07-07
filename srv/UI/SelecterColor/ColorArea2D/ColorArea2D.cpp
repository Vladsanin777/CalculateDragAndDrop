#include "ColorArea2D.hpp"

// ColorArea2D implementation (без изменений)
ColorArea2D::ColorArea2D(QWidget *parent) 
    : QWidget(parent) {
    setMinimumSize(200, 200);
}

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

void ColorArea2D::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    
    if (backgroundDirty || background.size() != size()) {
        updateBackground();
        backgroundDirty = false;
    }
    
    painter.drawImage(0, 0, background);
    
    int x = saturation * width();
    int y = (1 - value) * height();
    
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
    
    for (int y = 0; y < height(); ++y) {
        qreal val = 1.0 - static_cast<qreal>(y) / height();
        for (int x = 0; x < width(); ++x) {
            qreal sat = static_cast<qreal>(x) / width();
            QColor color;
            color.setHsvF(baseHue, sat, val);
            background.setPixelColor(x, y, color);
        }
    }
}

void ColorArea2D::selectColorAt(const QPoint &pos) {
    qreal sat = qBound(0.0, static_cast<qreal>(pos.x()) / width(), 1.0);
    qreal val = qBound(0.0, 1.0 - static_cast<qreal>(pos.y()) / height(), 1.0);
    
    setSaturationValue(sat, val);
    
    if (colorChangedCallback) {
        colorChangedCallback(baseHue, sat, val);
    }
}