#include "UI/Theme/Gradient/Gradient.hpp"
namespace Theme {
    inline Gradient::Gradient(QGradient::Type \
        type, QGradient::Spread spread, QGradient::CoordinateMode mode)
        : _gradient{}, _type{type}, _spread{spread}, _mode{mode} \
        {return;}
    inline QGradient::Type Gradient::getType(void) {
        return _type;
    }
    inline void Gradient::setType(QGradient::Type type) {
        _type = type; return;
    }
    inline QGradient::Spread Gradient::getSpread(void) {
        return _spread;
    }
    inline void Gradient::setSpread(QGradient::Spread spread) {
        _spread = spread; return;
    }
    inline QGradient::CoordinateMode Gradient::getMode(void) {
        return _mode;
    }
    inline void Gradient::setMode(QGradient::CoordinateMode mode) {
        _mode = mode; return;
    }
    inline void Gradient::setPointAt(qreal position, QColor color, bool isSelection) {
        _gradient.emplace_back(position, color); return;
    }
    inline void Gradient::setPointAt(GradientPoint point) {
        _gradient.push_back(point); return;
    }
    inline Gradient &Gradient::operator<<(GradientPoint point) {
        _gradient.push_back(point); return *this;
    }
    inline GradientPoint& Gradient::operator[](size_t index) {
        return _gradient[index];
    }
    inline size_t Gradient::size(void) {
        return _gradient.size();
    }
    inline GradientPointsIt Gradient::begin(void) {
        return _gradient.begin();
    }
    inline GradientPointsIt Gradient::end(void) {
        return _gradient.end();
    }
    inline void Gradient::insert(GradientPointsIt it, \
        GradientPoint point) {
        _gradient.insert(it, point); return;
    }
    inline void Gradient::insert(GradientPointsIt it) {
        _gradient.erase(it); return;
    }
    inline void Gradient::erase(GradientPointsIt it) {
        _gradient.erase(it); return;
    }
    inline size_t Gradient::addPoint(GradientPoint point, \
        size_t index) {
        insert(begin() + index, point);
        return index;
    }
}
