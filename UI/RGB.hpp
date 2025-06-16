#pragma once
#include "UI.hpp"
#include <functional>

namespace RGB {
    inline RGBA::RGBA(byte red, byte green, \
        byte blue) noexcept : _red{red}, \
        _green{green}, _blue{blue} {return;}
    inline void RGBA::set(byte red, byte green, \
        byte blue) noexcept {
        _red = red, _green = green, _blue = blue;
        if (_func) _func(red, green, blue);
        return;
    }
    inline void RGBA::setFunc( \
        std::function<void(byte, byte, byte)> func \
    ) noexcept { _func = func; return; }
    inline byte RGBA::red(void) const noexcept {return _red;}
    inline byte RGBA::green(void) const noexcept {return _green;}
    inline byte RGBA::blue(void) const noexcept {return _blue;}
    inline byte RGBA::alpha(void) const noexcept {return _alpha;}

    inline PRGBA::PRGBA(double pointer, \
        byte red, byte green, \
        byte blue, byte alpha) noexcept : \
        _pointer{pointer}, \
        RGBA{red, green, blue, alpha} {}
    inline PRGBA::pointer(void) const noexcept {return _pointer;}

    inline PRGBAS::PRGBAS( \
        const std::vector<PRGBA> prgbas \
    ) noexcept : _prgbas{prgbas} {return;}

    PRGBA &PRGBAS::operator[]( \
        size_t index) noexcept {return _rgbs[index];}

    inline Slider::Slider(byte number, \
        std::function<void(void)> func, \
        Channel *parent) noexcept : \
        QSlider{Qt::Horizontal} 
    {
        setRange(0, 255);
        setValue(number);
        connect(this, &QSlider::valueChanged, \
            [parent, this, func](void) {
                char *label {nullptr};
                asprintf(&label, "%hhu", (byte)this->value());
                parent->label()->setText(label);
                func();
            });
    }
    inline Channel::Channel(byte number, \
        std::function<void(void)> func) noexcept : \
        QBoxLayout{QBoxLayout::LeftToRight}, \
        _label{new Label::Label{number, fontSizeLabelRGB}}, \
        _slider{new Slider{number, func, this}} {
        addWidget(_label);
        addWidget(_slider);
    }
    inline Slider *Channel::slider(void) noexcept {
        return _slider;
    }
    inline Label::Label *Channel::label(void) noexcept {
        return _label;
    }
    inline Color::Color(Window::Window *window, \
        const RGB& rgb, \
        std::function<void(void)> funcApplyValue, \
        std::function<void( \
            std::function<void(byte, byte, byte)>)> func) \
            noexcept : QBoxLayout{QBoxLayout::TopToBottom}, \
        _redC{new Channel{rgb.red(), funcApplyValue}}, \
        _greenC{new Channel{rgb.green(), funcApplyValue}}, \
        _blueC{new Channel{rgb.blue(), funcApplyValue}}, \
        _red{_redC->slider()}, _green{_greenC->slider()}, \
        _blue{_blueC->slider()}
    {
        func(std::bind(&Color::setValue, this, \
            std::placeholders::_1, \
            std::placeholders::_2, \
            std::placeholders::_3));
        addLayout(_redC), \
        addLayout(_greenC), \
        addLayout(_blueC);
        addSpacing(20);
    }
    inline void Color::setValue(byte red, byte green, byte blue \
    ) noexcept {
        _red->setValue(red), \
        _green->setValue(green), \
        _blue->setValue(blue);
    }
    inline byte Color::getRedValue(void) noexcept {
        return _red->value();
    }
    inline byte Color::getGreenValue(void) noexcept {
        return _green->value();
    }
    inline byte Color::getBlueValue(void) noexcept {
        return _blue->value();
    }
    inline Gradient::Gradient(Window::Window *window) noexcept :
        QGridLayout{},
        _first(new Color(
            window, 
            window->getRGB0(),
            [this](void){this->applyGradient();},
            [window](std::function<void(byte, byte, byte)> f) {
                window->setFuncRGB0(f);
            }
        )),
        _second(new Color(
            window,
            window->getRGB1(),
            [this](void){this->applyGradient();},
            [window](std::function<void(byte, byte, byte)> f) {
                window->setFuncRGB1(f);
            }
        )), _window{window}
    {
        addLayout(_first, 0, 0, 1, 1), \
        addLayout(_second, 0, 1, 1, 1);
    }

    inline void Gradient::applyGradient(void) noexcept {
        _window->setRGB( \
            _first->getRedValue(), _first->getGreenValue(), \
            _first->getBlueValue(), _second->getRedValue(), \
            _second->getGreenValue(), _second->getBlueValue());
    }
}