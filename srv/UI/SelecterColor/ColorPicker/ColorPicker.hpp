#pragma once

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
    class ColorPiker;
}
namespace SelecterColor {
    class ColorPicker : public QWidget {
    private:
        SelecterColor::HueSlider *hueSlider;      // Вертикальный слайдер для цвета (Hue)
        SelecterColor::ColorArea2D *colorArea;  // 2D-область для насыщенности и яркости
        SelecterColor::AlphaSlider *alphaSlider;    // Вертикальный слайдер для прозрачности
        QColor currentColor;
        std::function<void(const QColor&)> colorChangedCallback;
    public:
        explicit ColorPicker(QColor color, QWidget *parent = nullptr);
        QColor color() const;
        void setColorChangedCallback(std::function<void(const QColor&)> callback);
        void updateAlphaSliderColor();
    private:
        void handleHueChanged(int hueValue);
        void handleAreaChanged(qreal hue, qreal sat, qreal val);
        void handleAlphaChanged(int alphaValue);
        void updateColor();
    };
}