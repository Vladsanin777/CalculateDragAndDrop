#include "UI/SelecterColor/ColorPicker/ColorPicker.hpp"
//#include "UI/SelecterColor/HueSlider/HueSlider.cpp"
//#include "UI/SelecterColor/AlphaSlider/AlphaSlider.cpp"
//#include "UI/SelecterColor/ColorArea2D/ColorArea2D.cpp"
#include <QHBoxLayout>
namespace SelecterColor {
    // ColorPicker implementation (с изменениями для HueSlider)
    ColorPicker::ColorPicker(QColor * &color, QWidget *parent) 
        : QWidget(parent), _currentColor(color) {
        QHBoxLayout *mainLayout = new QHBoxLayout(this);
        mainLayout->setSpacing(15);
        

        // Создаем вертикальный слайдер для Alpha
        _alphaSlider = new AlphaSlider(0, color, 100, this, this);
        
        // Создаем 2D область для Saturation/Value
        _colorArea2D = new ColorArea2D{color, _alphaSlider, this};
        mainLayout->addWidget(_colorArea2D);
        
        mainLayout->addWidget(_alphaSlider);

        // Создаем кастомный слайдер для Hue
        _hueSlider = new HueSlider(0, color, 359, _colorArea2D, this);
        mainLayout->addWidget(_hueSlider);   

        // Начальные значения

        //_colorArea->setBaseHue(color->hueF());
        //_colorArea->setBaseHue(0.0);
        //_colorArea->setSaturationValue(color->saturationF(), color->valueF());
        //_colorArea->setSaturationValue(1.0, 1.0);
        //_colorArea->update();
        
        //updateAlphaSliderColor();

        // Подключаем обработчики изменений
        //QObject::connect(_hueSlider, &HueSlider::valueChanged, \
                        [this](int value) { handleHueChanged(value); });
        
        //_colorArea->setColorChangedCallback( \
            [this](qreal h, qreal s, qreal v) { handleAreaChanged(h, s, v); });
        
        //QObject::connect(_alphaSlider, &QSlider::valueChanged, \
                        [this](int value) { handleAlphaChanged(value); });
        
        //updateColor();
    }

    /*
    QColor ColorPicker::color() const {
        return *_currentColor;
    }
    void ColorPicker::setColorChangedCallback(std::function<void(const QColor&)> callback) {
        _colorChangedCallback = callback;
    }
    */

    // Новый метод для обновления цвета в альфа-слайдере
    /*
    void ColorPicker::updateAlphaSliderColor() {
        // Берем текущий цвет из 2D-области (без альфа)
        QColor baseColor {_colorArea->getCurrentColor()};
        _alphaSlider->setBaseColor(baseColor);
    }

    void ColorPicker::handleHueChanged(int hueValue) {
        qreal hue = 1.0 - hueValue / 359.0;
        _colorArea->setBaseHue(hue);
        updateColor();
    }

    void ColorPicker::handleAreaChanged(qreal hue, qreal sat, qreal val) {
        _alphaSlider->setBaseColor(QColor::fromHsvF(hue, sat, val));
        updateColor();
    }

    void ColorPicker::handleAlphaChanged(int alphaValue) {
        updateColor();
    }

    void ColorPicker::updateColor() {
        qreal h = _hueSlider->value() / 359.0;
        qreal s = _colorArea->getSaturation();
        qreal v = _colorArea->getValue();
        qreal a = _alphaSlider->value() / 100.0;
        
        _currentColor->setHsvF(h, s, v, a);
        
        if (_colorChangedCallback) {
            _colorChangedCallback(*_currentColor);
        }
    }
    */
}
