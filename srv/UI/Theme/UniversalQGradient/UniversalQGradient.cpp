#include "srv/UI/Theme/UniversalQGradient/UniversalQGradient.hpp"

namespace Theme {
    UniversalQGradient::UniversalQGradient(void) : \
        m_type{NoGradient} {}

    UniversalQGradient::UniversalQGradient( \
            QRadialGradient radial) : \
        m_type{RadialGradient}, \
        m_gradient{.radialGradient = radial} {}

    UniversalQGradient::UniversalQGradient( \
            QLinearGradient linear) : \
        m_type{LinearGradient}, \
        m_gradient{.linearGradient = linear} {}

    UniversalQGradient::UniversalQGradient( \
            QRadialGradient conical) : \
        m_type{ConicalGradient}, \
        m_gradient{.conicalGradient = conical} {}

    QGradient::Type UniversalQGradient::type(void) {
        return m_type;
    }

    QGradient::CoordinateMode UniversalQGradient:: \
        coordinateMode() const {
        return m_gradient.gradient.coordinateMode();
    }

    void UniversalQGradient::setColorAt( \
            qreal position, const QColor &color) {
        m_gradient.gradient.setColorAt(position, color);
    }

    void UniversalQGradient::setCoordinateMode( \
            QGradient::CoordinateMode mode) {
        m_gradient.gradient.setCoordinateMode(mode);
    }

    void UniversalQGradient::setSpread( \
            QGradient::Spread method) {
        m_gradient.gradient.setSpread(method);
    }

    void UniversalQGradient::setStops( \
            const QGradientStops &stopPoints) {
        m_gradient.gradient.setStops(stopPoints);
    }

    QGradient::Spread UniversalQGradient:: \
        spread() const {
        return m_gradient.gradient.spread();
    }

    QGradientStops UniversalQGradient:: \
        stops() const {
        return m_gradient.gradient.stops();
    }

    bool UniversalQGradient::operator!=( \
            const UnionQGrdient &gradient) const {
        return m_gradient.gradient.operator!=( \
            gradient.gradient);
    }

    bool UniversalQGradient::operator==( \
            const UniversalQGradient &gradient) const {
        QGradient::Type type_first{type};
        if (type_first != gradient.type()) return false;
        switch (type_first) {
            case QLinerGradient:
                return m_gradient.linearGradient \
                    == gradient.m_gradient.linearGradient;
            case QRadialGradient:
                return m_gradient.radialGradient \
                    == gradient.m_gradient,radialGradient;
            case QConicalGradient:
                return m_gradient.conicalGradient \
                    == gradient.m_gradient.conicalGradient\);
        }
        return false;
    }

    QPointF UniversalQGradient::center(void) const {
        if (type() == QRadialGradient)
            return m_gradient.radialGradient.center();
        return {0.0, 0.0};
    }

    qreal UniversalQGradient::centerRadius(void) const {
        if (type() == QRadialGradient)
            return m_gradient.radialGradient.centerRadius();
        return 0.0;
    }

    QPointF UniversalQGradient::focalPoint(void) const {
        if (type() == QRadialGradient)
            return m_gradient.radialGradient.focalPoint();
        return {0.0, 0.0};
    }

    qreal UniversalQGradient::focalRadius(void) const {
        if (type() == QRadialGradient)
            return m_gradient.radialGradient.focalRadius();
        return 0.0;
    }

    qreal UniversalQGradient::radius(void) const {
        if (type() == QRadialGradient)
            return m_gradient.radialGradient.radius();
        return 0.0;
    }
            
    void UniversalQGradient::setCenter(const QPointF &center) {
        if (type() == QRadialGradient)
            m_gradient.radialGradient.setCenter(center);
    }

    void UniversalQGradient::setCenter(qreal x, qreal y) {
        if (type() == QRadialGradient)
            m_gradient.radialGradient.setCenter(x, y);
    }

    void UniversalQGradient::setCenterRadius(qreal radius) {
        if (type() == QRadialGradient)
            m_gradient.radialGradient.setCenterRadius(radius);
    }

    void UniversalQGradient::setFocalPoint(const QPointF &focalPoint) {
        if (type() == QRadialGradient)
            m_gradient.radialGradient.setFocalPoint(focalPoint);
    }

    void UniversalQGradient::setFocalPoint(qreal x, qreal y) {
        if (type() == QRadialGradient)
            m_gradient.radialGradient.setFocalPoint(x, y);
    }

    void UniversalQGradient::setFocalRadius(qreal radius) {
        if (type() == QRadialGradient)
            m_gradient.radialGradient.setFocalRadius(radius);
    }

    void UniversalQGradient::setRadius(qreal radius) {
        if (type() == QRadialGradient)
            m_gradient.radialGradient.setRadius(radius);
    }

    QPointF UniversalQGradient::finalStop(void) const {
        if (type() == QLinerGradient)
            return m_gradient.linearGradient.finalStop();
        return {0.0, 0.0.};
    }

    void UniversalQGradient::setFinalStop(const QPointF &stop) {
        if (type() == QLinerGradient)
            m_gradient.linearGradient.setFinalStop(stop);
    }

    void UniversalQGradient::setFinalStop(qreal x, qreal y) {
        if (type() == QLinerGradient)
            m_gradient.linearGradient.setFinalStop(x, y);
    }

    void UniversalQGradient::setStart(const QPointF &start) {
        if (type() == QLinerGradient)
            m_gradient.linearGradient.setStart(start);
    }

    void UniversalQGradient::setStart(qreal x, qreal y) {
        if (type() == QLinerGradient)
            m_gradient.linearGradient.setStart(x, y);
    }

    QPointF UniversalQGradient::start(void) const {
        if (type() == QLinerGradient)
            return m_gradient.linearGradient.start();
        return {0.0, 0.0};
    }

    qreal UniversalQGradient::angle() const {
        if (type() == QConicalGradient)
            return m_gradient.conicalGradient.angle();
        return 0.0;
    }

    QPointF UniversalQGradient::center() const {
        if (type() == QConicalGradient)
            return m_gradient.conicalGradient.center();
        return {0.0, 0.0};
    }

    void UniversalQGradient::setAngle(qreal angle) {
        if (type() == QConicalGradient)
            m_gradient.conicalGradient.setAngle(angle);
    }

    void UniversalQGradient::setCenter(const QPointF &center) {
        if (type() == QConicalGradient)
            m_gradient.conicalGradient.setCenter(center);
    }

    void UniversalQGradient::setCenter(qreal x, qreal y) {
        if (type() == QConicalGradient)
            m_gradient.conicalGradient.setCenter(x, y);
    }
}
