#include "UI/Theme/GradientPoint/GradientPoint.hpp"

namespace Theme {
    inline GradientPoint::GradientPoint( \
        qreal position, QColor color \
    ) : _position{position}, _color{color} \
    { return; }

    inline qreal &GradientPoint::getPosition(void) {
        return _position;
    }
    inline qreal GradientPoint::getPosition(void) const {
        return _position;
    }
    inline void GradientPoint::setPosition(qreal position) {
        _position = position;
        return;
    }
    inline QColor &GradientPoint::getColor(void) {
        return _color;
    }
    inline QColor GradientPoint::getColor(void) const {
        return _color;
    }
    inline void GradientPoint::setColor(QColor color) {
        _color = color;
        return;
    }
}
