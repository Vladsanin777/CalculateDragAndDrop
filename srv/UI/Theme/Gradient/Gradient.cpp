#include "UI/Theme/Gradient/Gradient.hpp"
#include <iostream>

std::ostream &operator<<(std::ostream &out, std::vector<Theme::GradientPoint> &vec) {
    for (auto element : vec)
        out << element << ' ';
    out << std::endl;
    return out;
}

namespace Theme {

    Gradient::Gradient(QGradient::Type type, \
            const QGradientStops & stops) \
        : m_stops{stops}, m_type{type} {return;}

    Gradient::Gradient(QGradient::Type type, const QColor & color) \
        : m_stops{1, QGradientStop{0.0, color}}, \
                m_type{type} {return;}

    Gradient::Gradient(QGradient::Type type) \
        : m_type{type} {return;}

    Gradient::Gradient(void) {return;}

    Gradient::operator QGradient(void) const {
        QGradient gradient{};
        switch (m_type) {
            QGradient::LinearGradient:
                gradient = QLinearGradient \
                        {m_pointStartLinear, m_pointStopLinear};
                break;
            QGradient::RadialGradient
                gradient = QRadialGradient \
                        {m_pointCenterRadial, m_radiusCentralRadial, \
                            m_pointFocalRadial, m_radiusFocalRadial};
                break;
            QGradient::ConicalGradient:
                gradient = QConicalGradient \
                        {m_pointCenterConical, m_angleConical};
                break;
        }
        gradient.setSpread(m_spread);
        gradient.setStops(m_stops);
        gradient.setCoordinateMode(m_coordinateMode);
        gradient.setInterpolationMode(m_interpolationMode);
        return gradient;
    }

    Gradient::operator QGradientStops(void) const {
        return m_stops;
    }

    Gradient::operator QGradient::Type(void) const {
        return m_type;
    }

    Gradient::operator QGradient::Spread(void) const {
        return m_spread;
    }

    Gradient::operator QGradient::CoordinateMode(void) const {
        return m_coordinateMode;
    }

    Gradient::operator QGradient::InterpolationMode(void) const {
        return m_interpolationMode;
    }

    QGradient::Type Gradient::type(void) const {
        return m_type;
    }

    void Gradient::setType(QGradient::Type type) {
        m_type = type; return;
    }

    const QGradient::Spread & Gradient::spread(void) const {
        return m_spread;
    }

    void Gradient::setSpread(QGradient::Spread spread) {
        m_spread = spread; return;
    }

    const QGradient::CoordinateMode & Gradient::\
            coordinateMode(void) const {
        return m_coordinateMode;
    }

    void Gradient::setCoordinateMode( \
            QGradient::CoordinateMode coordinateMode) const {
        m_coordinateMode = coordinateMode; return;
    }

    void Gradient::setPointAt(QGradientStop point) {
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

    size_t Gradient::addPoint(GradientStop point, \
        size_t index) {
        printf("index addPoint in Theme Gradient: %lu", index);
        insert(begin() + index, point);
        return index;
    }

    void Gradient::update(void) {
        if (_gradient) delete _gradient;
        switch (_type) {
            QGradient::LinearGradient:
                _gradient = new QLinearGradient{ \
                    qreal(0.0), qreal(0.0), qreal(1.0), qreal(1.0)};
                break;
            QGradient::QRadialGradient
                _gradient = new QRadialGradient{ \
                    qreal(0.5), qreal(0.5), qreal(0.5), \
                        qreal(0.5), qreal(0.5)};
                break;
            QGradient::QConicalGradient:
                _gradient = new QConicalGradient{ \
                    qreal(0.5), qreal(0.5), qreal(0.0)};
                break;
            QGradient::NoGradient:
                _gradient = new QGradient{};
                break;
        }
        _gradient.setStops(stops);
    }
}
