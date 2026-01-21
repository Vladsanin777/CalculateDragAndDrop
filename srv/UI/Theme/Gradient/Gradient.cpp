#include "UI/Theme/Gradient/Gradient.hpp"
#include <iostream>

std::ostream &operator<<(std::ostream &out, std::vector<Theme::GradientPoint> &vec) {
    for (auto element : vec)
        out << element << ' ';
    out << std::endl;
    return out;
}

namespace Theme {

    // constructors

    Gradient::Gradient(QGradient::Type type, \
            const QGradientStops & stops) \
        : m_stops{stops}, m_type{type} {return;}

    Gradient::Gradient(QGradient::Type type, const QColor & color) \
        : m_stops{1, QGradientStop{0.0, color}}, \
                m_type{type} {return;}

    Gradient::Gradient(QGradient::Type type) \
        : m_type{type} {return;}

    Gradient::Gradient(void) {return;}

    // Converters

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

    // Basic QGradient

    QGradient::Type Gradient::type(void) const {
        return m_type;
    }

    void Gradient::setType(QGradient::Type type) {
        m_type = type;
        emit update();
    }

    const QGradient::Spread & Gradient::spread(void) const {
        return m_spread;
    }

    void Gradient::setSpread(QGradient::Spread spread) {
        m_spread = spread;
        emit update();
    }

    const QGradient::CoordinateMode & Gradient:: \
            coordinateMode(void) const {
        return m_coordinateMode;
    }

    void Gradient::setCoordinateMode( \
            QGradient::CoordinateMode coordinateMode) const {
        m_coordinateMode = coordinateMode;
        emit update();
    }

    const QGradient::InterpolationMode & Gradient:: \
            interpolationMode(void) const {
        return m_interpolationMode;
    }

    void Gradient::setInterpolationMode(QGradient::InterpolationMode \
            interpolationMode) {
        m_interpolationMode = interpolationMode;
        emit update();
    }

    // QLinearGradient

    QPointF Gradient::startLinear(void) const {
        return m_pointStartLinear;
    }

    void Gradient::setStartLinear(const QPointF &start) {
        m_pointStartLinear = start; return;
        if (m_type == QGradient::LinerGradient)
            emit update();
    }

    QPointF Gradient::finalStopLinear(void) const {
        return m_pointStopLinear;
    }

    void Gradient::setFinalStopLinear(const QPointF &stop) {
        m_pointStopLinear = stop;
        if (m_type == QGradient::LinerGradient)
            emit update();
    }

    // QRadialGradient

    QPointF Gradient::centerRadial(void) const {
        return m_radiusCentralRadial;
    }

    void Gradient::setCenterRadial(const QPointF &center) {
        m_radiusCentralRadial = center;
        if (m_type == QGradient::RadialGradient)
            emit update();
    }

    QPointF Gradient::focalPointRadial(void) const {
        return m_pointFocalRadial;
    }

    void Gradient::setFocalPointRadial(const QPointF &focalPoint) {
        m_pointFocalRadial = focalPoint;
        if (m_type == QGradient::RadialGradient)
            emit update();
    }

    qreal Gradient::radiusRadial(void) const {
        return m_radiusRadial;
    }

    void Gradient::setRadiusRadial(qreal radius) {
        m_radiusRadial = radius;
        if (m_type == QGradient::RadialGradient)
            emit update();
    }

    qreal Gradient::centerRadiusRadial(void) const {
        return m_radiusCentralRadial;
    }

    void Gradient::setCenterRadiusRadial(qreal radius) {
        m_radiusCentralRadial = radius;
        if (m_type == QGradient::RadialGradient)
            emit update();
    }

    qreal Gradient::focalCenterRadiusRadial(void) const {
        return m_pointFocalRadial;
    }

    void Gradient::setFocalRadiusRadial(qreal radius) {
        m_pointFocalRadial = radius;
        if (m_type == QGradient::RadialGradient)
            emit update();
    }

    // QConicalGradient

    QPointF Gradient::centerConical(void) const {
        return m_pointCenterConical;
    }

    void Gradient::setCenterConical(const QPointF &center) {
        m_pointCenterConical = center;
        if (m_type == QGradient::ConicalGradient)
            emit update();
    }

    qreal Gradient::angleConical(void) const {
        return m_angleConical;
    }

    void Gradient::setAngleConical(qreal angle) {
        m_angleConical = angle;
        if (m_type == QGradient::ConicalGradient)
            emit update();
    }

    // Operations Stops

    void Gradient::createStop(qsizetype index) {
        m_stops.insert(index, \
                QPointF{(m_stops[index - 1].position + \
                m_stops[index].position) / 2, QColor{ \
                (m_stops[index - 1].color.red() + \
                m_stops[index].color.red()) / 2, \
                (m_stops[index - 1].color.green() + \
                m_stops[index].color.green()) / 2, \
                (m_stops[index - 1].color.blue() + \
                m_stops[index].color.blue()) / 2, \
                (m_stops[index - 1].color.alpha() + \
                m_stops[index].color.alpha()) / 2}});
        emit update();
    }

    void Gradient::removeStop(qsizetype index) {
        m_stops.remove(index);
        emit update();
    }
}
