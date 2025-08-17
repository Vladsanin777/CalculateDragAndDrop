#pragma once
#include <vector>

#include <QGradient>

#include "UI/Theme/GradientPoint/GradientPoint.hpp"


namespace Theme {
    using GradientPoints = std::vector<GradientPoint>;
    using GradientPointsIt = GradientPoints::iterator;

    class Gradient {
    private:

        GradientPoints _gradient{};
        QGradient::Type _type{QGradient::LinearGradient};
        QGradient::Spread _spread{QGradient::PadSpread};
        QGradient::CoordinateMode _mode{QGradient::LogicalMode};

    public:
        Gradient(QGradient::Type type = QGradient::LinearGradient, \
            QGradient::Spread spread = QGradient::PadSpread, \
            QGradient::CoordinateMode mode = QGradient::LogicalMode);
        QGradient::Type getType(void);
        void setType(QGradient::Type type);
        QGradient::Spread getSpread(void);
        void setSpread(QGradient::Spread spread);
        QGradient::CoordinateMode getMode(void);
        void setMode(QGradient::CoordinateMode mode);
        void setPointAt(qreal position, QColor color, bool isSelection);
        void setPointAt(GradientPoint point);
        Gradient &operator<<(GradientPoint point);
        GradientPoint& operator[](size_t index);
        size_t size(void);
        GradientPointsIt begin(void);
        GradientPointsIt end(void);
        void insert(GradientPointsIt it, \
            GradientPoint point);
        void insert(GradientPointsIt it);
        void erase(GradientPointsIt it);
        size_t addPoint(GradientPoint point, \
            size_t index);
    };
}
