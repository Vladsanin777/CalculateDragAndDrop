#include "hueslider.cpp"
#include "alphaslider.cpp"
#include "colorpicker.h"

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

// ColorPicker implementation (с изменениями для HueSlider)
ColorPicker::ColorPicker(QWidget *parent) 
    : QWidget(parent), currentColor(Qt::red) {
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(15);
    

    
    // Создаем 2D область для Saturation/Value
    colorArea = new ColorArea2D;
    mainLayout->addWidget(colorArea, 3);
    
    // Создаем вертикальный слайдер для Alpha
    alphaSlider = new AlphaSlider(0, 100, 100);
    mainLayout->addWidget(alphaSlider);

    // Создаем кастомный слайдер для Hue
    hueSlider = new HueSlider(0, 359, 359);
    mainLayout->addWidget(hueSlider);   

    // Начальные значения
    colorArea->setBaseHue(0.0);
    colorArea->setSaturationValue(1.0, 1.0);
    
    updateAlphaSliderColor();

    // Подключаем обработчики изменений
    QObject::connect(hueSlider, &HueSlider::valueChanged, 
                     [this](int value) { handleHueChanged(value); });
    
    colorArea->setColorChangedCallback(
        [this](qreal h, qreal s, qreal v) { handleAreaChanged(h, s, v); });
    
    QObject::connect(alphaSlider, &QSlider::valueChanged,
                     [this](int value) { handleAlphaChanged(value); });
    
    updateColor();
}

QColor ColorPicker::color() const {
    return currentColor;
}

void ColorPicker::setColorChangedCallback(std::function<void(const QColor&)> callback) {
    colorChangedCallback = callback;
}

// Новый метод для обновления цвета в альфа-слайдере
void ColorPicker::updateAlphaSliderColor() {
    // Берем текущий цвет из 2D-области (без альфа)
    QColor baseColor = colorArea->getCurrentColor();
    alphaSlider->setBaseColor(baseColor);
}

void ColorPicker::handleHueChanged(int hueValue) {
    qreal hue = 1.0 - hueValue / 359.0;
    colorArea->setBaseHue(hue);
    updateColor();
}

void ColorPicker::handleAreaChanged(qreal hue, qreal sat, qreal val) {
    alphaSlider->setBaseColor(QColor::fromHsvF(hue, sat, val));
    updateColor();
}

void ColorPicker::handleAlphaChanged(int alphaValue) {
    updateColor();
}

void ColorPicker::updateColor() {
    qreal h = hueSlider->value() / 359.0;
    qreal s = colorArea->getSaturation();
    qreal v = colorArea->getValue();
    qreal a = alphaSlider->value() / 100.0;
    
    currentColor.setHsvF(h, s, v, a);
    
    if (colorChangedCallback) {
        colorChangedCallback(currentColor);
    }
}