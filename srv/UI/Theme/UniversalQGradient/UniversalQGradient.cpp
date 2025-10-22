#include <QRadialGradient>
#include <QLinerGradient>
#include <QConicalGradient>
#include <QGradient>

union unionQGradient {
    QRadialGradient  radialGradient;
    QLinearGradient  linearGradient;
    QConicalGradient conicalGradient;
    QGradient        gradient;
};

class UniversalQGrdient {
private:
    QGradient::Type m_type;
    unionQGradient m_gradient;
public:
    UnionQGrdient(void) : m_type{NoGradient} {}

    UnionQGrdient(QRadialGradient radial) : \
        m_type{RadialGradient}, \
        m_gradient{.radialGradient = radial} {}

    UnionQGrdient(QLinearGradient linear) : \
        m_type{LinearGradient}, \
        m_gradient{.linearGradient = linear} {}

    UnionQGrdient(QRadialGradient conical) : \
        m_type{ConicalGradient}, \
        m_gradient{.conicalGradient = conical} {}

    QGradient::Type type(void) {
        return m_type;
    }

    QGradient::CoordinateMode coordinateMode() const {
        return m_gradient.gradient.coordinateMode();
    }

    void setColorAt(qreal position, const QColor &color) {
        m_gradient.gradient.setColorAt(position, color);
    }

    void setCoordinateMode(QGradient::CoordinateMode mode) {
        m_gradient.gradient.setCoordinateMode(mode);
    }

    void setSpread(QGradient::Spread method) {
        m_gradient.gradient.setSpread(method);
    }

    void setStops(const QGradientStops &stopPoints) {
        m_gradient.gradient.setStops(stopPoints);
    }
    QGradient::Spread spread() const {
        return m_gradient.gradient.spread();
    }

    QGradientStops stops() const {
        return m_gradient.gradient.stops();
    }

    bool operator!=(const UnionQGrdient &gradient) const {
        return m_gradient.gradient.operator!=( \
            gradient.gradient);
    }

    bool operator==(const UnionQGrdient &gradient) const {
        return m_gradient.gradient.operator==( \
            gradient.gradient);
    }

    QPointF center(void) const {
        if (type() == QRadialGradient)
            return m_gradient.radialGradient.center();
        return {0.0, 0.0};
    }

    qreal centerRadius(void) const {
        if (type() == QRadialGradient)
            return m_gradient.radialGradient.centerRadius();
        return 0.0;
    }

    QPointF focalPoint(void) const {
        if (type() == QRadialGradient)
            return m_gradient.radialGradient.focalPoint();
        return {0.0, 0.0};
    }

    qreal focalRadius(void) const {
        if (type() == QRadialGradient)
            return m_gradient.radialGradient.focalRadius();
        return 0.0;
    }

    qreal radius(void) const {
        if (type() == QRadialGradient)
            return m_gradient.radialGradient.radius();
        return 0.0;
    }
            
    void setCenter(const QPointF &center) {
        if (type() == QRadialGradient)
            m_gradient.radialGradient.setCenter(center);
    }

    void setCenter(qreal x, qreal y) {
        if (type() == QRadialGradient)
            m_gradient.radialGradient.setCenter(x, y);
    }

    void setCenterRadius(qreal radius) {
        if (type() == QRadialGradient)
            m_gradient.radialGradient.setCenterRadius(radius);
    }

    void setFocalPoint(const QPointF &focalPoint) {
        if (type() == QRadialGradient)
            m_gradient.radialGradient.setFocalPoint(focalPoint);
    }

    void setFocalPoint(qreal x, qreal y) {
        if (type() == QRadialGradient)
            m_gradient.radialGradient.setFocalPoint(x, y);
    }

    void setFocalRadius(qreal radius) {
        if (type() == QRadialGradient)
            m_gradient.radialGradient.setFocalRadius(radius);
    }

    void setRadius(qreal radius) {
        if (type() == QRadialGradient)
            m_gradient.radialGradient.setRadius(radius);
    }

    QPointF finalStop(void) const {
        if (type() == QLinerGradient)
            return m_gradient.linearGradient.finalStop();
        return {0.0, 0.0.};
    }

    void setFinalStop(const QPointF &stop) {
        if (type() == QLinerGradient)
            m_gradient.linearGradient.setFinalStop(stop);
    }

    void setFinalStop(qreal x, qreal y) {
        if (type() == QLinerGradient)
            m_gradient.linearGradient.setFinalStop(x, y);
    }

    void setStart(const QPointF &start) {
        if (type() == QLinerGradient)
            m_gradient.linearGradient.setStart(start);
    }

    void setStart(qreal x, qreal y) {
        if (type() == QLinerGradient)
            m_gradient.linearGradient.setStart(x, y);
    }

    QPointF start(void) const {
        if (type() == QLinerGradient)
            return m_gradient.linearGradient.start();
        return {0.0, 0.0};
    }

    qreal angle() const {
        if (type() == QConicalGradient)
            return m_gradient.conicalGradient.angle();
        return 0.0;
    }

    QPointF center() const {
        if (type() == QConicalGradient)
            return m_gradient.conicalGradient.center();
        return {0.0, 0.0};
    }

    void setAngle(qreal angle) {
        if (type() == QConicalGradient)
            m_gradient.conicalGradient.setAngle(angle);
    }

    void setCenter(const QPointF &center) {
        if (type() == QConicalGradient)
            m_gradient.conicalGradient.setCenter(center);
    }

    void setCenter(qreal x, qreal y) {
        if (type() == QConicalGradient)
            m_gradient.conicalGradient.setCenter(x, y);
    }
};
