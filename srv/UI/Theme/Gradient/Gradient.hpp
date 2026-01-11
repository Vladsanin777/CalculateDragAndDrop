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

        

        // Operations Stops

        void createStop(size_t index);

        void addStop(GradientStop stop);

        void removeStop(size_t index);

    private:
        void update(void);
    };
}
