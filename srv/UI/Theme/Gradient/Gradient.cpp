#include "UI/Theme/Gradient/Gradient.hpp"
#include <iostream>

std::ostream &operator<<(std::ostream &out, std::vector<Theme::GradientPoint> &vec) {
    for (auto element : vec)
        out << element << ' ';
    out << std::endl;
    return out;
}

namespace Theme {
    Gradient::Gradient(QGradient::Type \
        type, QGradient::Spread spread, QGradient::CoordinateMode mode)
        : _gradient{}, _type{type}, _spread{spread}, _mode{mode} \
        {return;}
    QGradient::Type Gradient::getType(void) {
        return _type;
    }
    void Gradient::setType(QGradient::Type type) {
        _type = type; return;
    }
    QGradient::Spread Gradient::getSpread(void) {
        return _spread;
    }
    void Gradient::setSpread(QGradient::Spread spread) {
        _spread = spread; return;
    }
    QGradient::CoordinateMode Gradient::getMode(void) {
        return _mode;
    }
    void Gradient::setMode(QGradient::CoordinateMode mode) {
        _mode = mode; return;
    }
    void Gradient::setPointAt(qreal position, QColor color, bool isSelection) {
        _gradient.emplace_back(position, color); return;
    }
    void Gradient::setPointAt(GradientPoint point) {
        _gradient.push_back(point); return;
    }
    Gradient &Gradient::operator<<(GradientPoint point) {
        _gradient.push_back(point); return *this;
    }
    GradientPoint& Gradient::operator[](size_t index) {
        printf("Gradient operator[%lu] \n", index);
        return _gradient[index];
    }
    size_t Gradient::size(void) {
        return _gradient.size();
    }
    GradientPointsIt Gradient::begin(void) {
        return _gradient.begin();
    }
    GradientPointsIt Gradient::end(void) {
        return _gradient.end();
    }
    void Gradient::insert(GradientPointsIt it, \
        GradientPoint point) {
        _gradient.insert(it, point); return;
    }
    void Gradient::insert(GradientPointsIt it) {
        _gradient.erase(it); return;
    }
    void Gradient::erase(GradientPointsIt it) {
        _gradient.erase(it); return;
    }
    size_t Gradient::addPoint(GradientPoint point, \
        size_t index) {
        printf("index addPoint in Theme Gradient: %lu", index);
        insert(begin() + index, point);
        return index;
    }
    void Gradient::update(void) {
        if (_qgradient) delete _qgradient;
        switch (_type) {
            QGradient::LinearGradient:
                _qgradient = new QLinearGradient{
                /* TODO */
        }
    }
}
