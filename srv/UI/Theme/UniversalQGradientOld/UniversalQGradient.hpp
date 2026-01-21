#include <QRadialGradient>
#include <QLinerGradient>
#include <QConicalGradient>
#include <QGradient>

namespace Theme {
    union unionQGradient {
        QRadialGradient  radialGradient;
        QLinearGradient  linearGradient;
        QConicalGradient conicalGradient;
        QGradient        gradient;
    };

    class UniversalQGradient {
    private:
        QGradient::Type m_type;
        unionQGradient m_gradient;
    public:
        UniversalQGradient(void);
        UniversalQGradient(QRadialGradient radial);
        UniversalQGradient(QLinearGradient linear);
        UniversalQGradient(QRadialGradient conical);
        QGradient::Type type(void);
        QGradient::CoordinateMode coordinateMode() const;
        void setColorAt(qreal position, const QColor &color);
        void setCoordinateMode(QGradient::CoordinateMode mode);
        void setSpread(QGradient::Spread method);
        void setStops(const QGradientStops &stopPoints);
        QGradient::Spread spread() const;
        QGradientStops stops() const;
        bool operator!=(const UnionQGrdient &gradient) const;
        bool operator==(const UnionQGrdient &gradient) const;
        QPointF center(void) const;
        qreal centerRadius(void) const;
        QPointF focalPoint(void) const;
        qreal focalRadius(void) const;
        qreal radius(void) const;
        void setCenter(const QPointF &center);
        void setCenter(qreal x, qreal y);
        void setCenterRadius(qreal radius);
        void setFocalPoint(const QPointF &focalPoint);
        void setFocalPoint(qreal x, qreal y);
        void setFocalRadius(qreal radius);
        void setRadius(qreal radius);
        QPointF finalStop(void) const;
        void setFinalStop(const QPointF &stop);
        void setFinalStop(qreal x, qreal y);
        void setStart(const QPointF &start);
        void setStart(qreal x, qreal y);
        QPointF start(void) const;
        qreal angle() const;
        QPointF center() const;
        void setAngle(qreal angle);
        void setCenter(const QPointF &center);
        void setCenter(qreal x, qreal y);
    };
}
