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
        _alphaSlider = new AlphaSlider(0, color, 255, this, this);
        
        // Создаем 2D область для Saturation/Value
        _colorArea2D = new ColorArea2D{color, _alphaSlider, this};
        mainLayout->addWidget(_colorArea2D);
        
        mainLayout->addWidget(_alphaSlider);

        // Создаем кастомный слайдер для Hue
        _hueSlider = new HueSlider(0, color, 359, _colorArea2D, this);
        mainLayout->addWidget(_hueSlider);   
    }
}
