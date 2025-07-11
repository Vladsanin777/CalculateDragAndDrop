#include "ColorPicker.hpp"

namespace SelecterColor {
    // ColorPicker implementation (с изменениями для HueSlider)
    ColorPicker::ColorPicker(QColor color, QWidget *parent) 
        : QWidget(parent), currentColor(Qt::red) {
        QHBoxLayout *mainLayout = new QHBoxLayout(this);
        mainLayout->setSpacing(15);
        

        
        // Создаем 2D область для Saturation/Value
        colorArea = new SelecterColor::ColorArea2D;
        mainLayout->addWidget(colorArea, 3);
        
        // Создаем вертикальный слайдер для Alpha
        alphaSlider = new SelecterColor::AlphaSlider(0, 100, 100);
        mainLayout->addWidget(alphaSlider);

        // Создаем кастомный слайдер для Hue
        hueSlider = new SelecterColor::HueSlider(0, 359, 359);
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
}