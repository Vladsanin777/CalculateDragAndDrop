#include "UI.hpp"
#include <functional>

namespace RGB {
    inline Slider::Slider(byte number) noexcept : \
        QSlider{Qt::Horizontal} 
    {
        setRange(0, 255);
        setValue(number);
        //connect(slider, &QSlider::valueChanged,
            this, &Window::Window::set);
    }
    inline Color::Color(Window::Window *window, \
        const RGB& rgb, std::function<void( \
            std::function<void(byte, byte, byte)>)> func) \
            noexcept : QGridLayout{}, \
        _red {new Slider{rgb.red()}}, \
        _green {new Slider{rgb.green()}}, \
        _blue {new Slider{rgb.blue()}} 
    {
        func(std::bind(&Color::setValue, this, \
            std::placeholders::_1, \
            std::placeholders::_2, \
            std::placeholders::_3));
        addWidget(_red, 0, 0, 3, 1), \
        addWidget(_green, 1, 0, 3, 1), \
        addWidget(_blue, 2, 0, 3, 1);

    }
    inline void Color::setValue(byte red, byte green, byte blue \
    ) noexcept {
        _red->setValue(red), \
        _green->setValue(green), \
        _blue->setValue(blue);
    }
    inline Gradient::Gradient(Window::Window *window) noexcept :
        QGridLayout{},
        _first(new Color(
            window, 
            window->getRGB0(),
            [window](std::function<void(unsigned char, unsigned char, unsigned char)> f) {
                window->setFuncRGB0(f);
            }
        )),
        _second(new Color(
            window,
            window->getRGB1(),
            [window](std::function<void(unsigned char, unsigned char, unsigned char)> f) {
                window->setFuncRGB1(f);
            }
        ))
    {
        addLayout(_first, 0, 0, 1, 1), \
        addLayout(_second, 0, 1, 1, 1);
    }
}