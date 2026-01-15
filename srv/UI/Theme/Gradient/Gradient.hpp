#pragma once
#include <iostream>
#include <vector>

#include <QGradient>

#include "UI/Theme/GradientPoint/GradientPoint.hpp"


std::ostream &operator<<(std::ostream &out, std::vector<Theme::GradientPoint> &vec);

namespace Theme {
    //using GradientPoints = std::vector<GradientPoint>;
    using QGradientStopsIt = GradientStops::iterator;

    class Gradient {
    private:
        QGradientStops m_stops \
                {1, QGradientStop{0.0, QColor{}}};
        QGradient::Type m_type{QGradient::NoGradient};
        QGradient::Spread m_spread{QGradient::PadSpread};
        QGradient::CoordinateMode m_coordinateMode \
                {QGradient::LogicalMode};
        QGradient::InterpolationMode m_interpolationMode \
                {QGradient::ColorInterpolation};
        QPointF m_pointStartLinear{0.0, 0.0};
        QPointF m_pointStopLinear{1.0, 1.0};
        QPointF m_pointCenterRadial{0.0, 0.0};
        qreal m_radiusCentralRadial{1.0};
        QPointF m_pointFocalRadial{0.0, 0.0};
        qreal m_radiusFocalRadial{0.0};
        qreal m_radiusRadial{0.0};
        QPointF m_pointCenterConical{0.0, 0.0};
        qreal m_angleConical{0.0};

    public:

        // constructors

        Gradient(QGradient::Type type, const QGradientStops & stops);

        Gradient(QGradient::Type type, const QColor & color);

        Gradient(QGradient::Type type);

        Gradient(void);

        // Converters

        operator QGradient(void) const;

        operator QGradientStops(void) const;

        operator QGradient::Type(void) const;

        // Basic QGradient

        QGradient::Type type(void) const;

        void setType(QGradient::Type type);

        QGradient::Spread spread(void) const;

        void setSpread(QGradient::Spread spread);

        QGradient::CoordinateMode coordinateMode(void) const;

        void setCoordinateMode(QGradient::CoordinateMode \
                coordinateMode);

        QGradient::InterpolationMode interpolationMode(void) const;

        void setInterpolationMode(QGradient::InterpolationMode \
                interpolationMode);

        // QLinearGradient

        QPointF startLinear(void) const;

        void setStartLinear(const QPointF &start);

        QPointF finalStopLinear(void) const;

        void setFinalStopLinear(const QPointF &stop);

        // QRadialGradient

        QPointF centerRadial(void) const;

        void setCenterRadial(const QPointF &center);

        QPointF focalPointRadial(void) const;

        void setFocalPointRadial(const QPointF &focalPoint);

        qreal radiusRadial(void) const;

        void setRadiusRadial(qreal radius);

        qreal centerRadiusRadial(void) const;

        void setCenterRadiusRadial(qreal radius);

        qreal focalCenterRadiusRadial(void) const;

        void setFocalRadiusRadial(qreal radius);

        // QConicalGradient

        QPointF centerConical(void) const;

        void setCenterConical(const QPointF &center);

        qreal angleConical(void) const;

        void setAngleConical(qreal angle);

        // Operations Stops

        void createStop(qsizetype index);

        void removeStop(qsizetype index);
    };
}
