// #pragma once

#include <QWidget>
#include <QColor>
#include <functional>

#include "../HueSlider/HueSlider.cpp"
#include "../AlphaSlider/AlphaSlider.cpp"
#include "../ColorArea2D/ColorArea2D.cpp"

class QWidget;
class QColor;

namespace SelecterColor {
    class HueSlider;
    class AlphaSlider;
    class ColorArea2D;
}

namespace SelecterColor {
    class ColorPicker;
}
namespace SelecterColor {
    class ColorPicker : public QWidget {
    private:
        SelecterColor::HueSlider *_hueSlider;      // Вертикальный слайдер для цвета (Hue)
        SelecterColor::ColorArea2D *_colorArea2D;  // 2D-область для насыщенности и яркости
        SelecterColor::AlphaSlider *_alphaSlider;    // Вертикальный слайдер для прозрачности
        QColor * &_currentColor;
        std::function<void(const QColor&)> _colorChangedCallback;
    public:
        explicit ColorPicker(QColor * &color, QWidget *parent = nullptr);
        //void updateNode(void);
        //QColor color() const;
        //void setColorChangedCallback(std::function<void(const QColor&)> callback);
        //void updateAlphaSliderColor();
    private:
        //void handleHueChanged(int hueValue);
        //void handleAreaChanged(qreal hue, qreal sat, qreal val);
        //void handleAlphaChanged(int alphaValue);
        //void updateColor();
    };
}