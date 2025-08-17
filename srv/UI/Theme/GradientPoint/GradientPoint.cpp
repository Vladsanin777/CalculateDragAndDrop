#include "UI/Theme/GradientPoint/GradientPoint.hpp"

namespace Theme {
    GradientPoint::GradientPoint( \
        qreal position, QColor color \
    ) : _position{position}, _color{color} \
    { return; }

    qreal &GradientPoint::getPosition(void) {
        return _position;
    }
    qreal GradientPoint::getPosition(void) const {
        return _position;
    }
    void GradientPoint::setPosition(qreal position) {
        _position = position;
        return;
    }
    QColor &GradientPoint::getColor(void) {
        return _color;
    }
    QColor GradientPoint::getColor(void) const {
        return _color;
    }
    void GradientPoint::setColor(QColor color) {
        _color = color;
        return;
    }
}
