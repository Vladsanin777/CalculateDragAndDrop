#include "hueslider.h"

HueSlider::HueSlider(QWidget *parent)
    : QSlider(parent) {
    setOrientation(Qt::Vertical);
    setRange(0, 359);
    setMinimumSize(30, 100);
}

void HueSlider::updateGradient() {

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

void HueSlider::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    
    if (gradientDirty || gradientImage.size() != size()) {
        updateGradient();
        gradientDirty = false;
    }
    
    // Рисуем градиент с отступами по бокам
    painter.drawImage(1, 0, gradientImage.copy(1, 0, width() - 2, height()));
    
    qreal ratio = static_cast<qreal>(value()) / (maximum() - minimum());
    int yPos = (1 - ratio) * height();
    yPos = qBound(0, yPos - handleHeight/2, height() - handleHeight);
    
    // Рисуем ползунок на всю ширину виджета (включая отступы)
    QRect handleRect(0, yPos, width()-1, handleHeight);
    QPen pen(Qt::white, 1);
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(handleRect);
}

void HueSlider::mousePressEvent(QMouseEvent *event) {
    // Преобразуем позицию Y в значение слайдера
    int value = maximum() - (event->pos().y() * maximum() / height());
    setValue(qBound(minimum(), value, maximum()));
}

void HueSlider::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        int value = maximum() - (event->pos().y() * maximum() / height());
        setValue(qBound(minimum(), value, maximum()));
    }
}

void HueSlider::resizeEvent(QResizeEvent *event) {
    QSlider::resizeEvent(event);
    gradientDirty = true;
}