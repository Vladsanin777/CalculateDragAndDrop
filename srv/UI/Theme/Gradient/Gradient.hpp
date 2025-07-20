#pragma once
#include <vector>

#include <QGradient>

#include "../GradientPoint/GradientPoint.cpp"


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
        inline Gradient(QGradient::Type type = QGradient::LinearGradient, \
            QGradient::Spread spread = QGradient::PadSpread, \
            QGradient::CoordinateMode mode = QGradient::LogicalMode);
        inline QGradient::Type getType(void);
        inline void setType(QGradient::Type type);
        inline QGradient::Spread getSpread(void);
        inline void setSpread(QGradient::Spread spread);
        inline QGradient::CoordinateMode getMode(void);
        inline void setMode(QGradient::CoordinateMode mode);
        inline void setPointAt(qreal position, QColor color, bool isSelection);
        inline void setPointAt(GradientPoint point);
        inline Gradient &operator<<(GradientPoint point);
        inline GradientPoint& operator[](size_t index);
        inline size_t size(void);
        inline GradientPointsIt begin(void);
        inline GradientPointsIt end(void);
        inline void insert(GradientPointsIt it, \
            GradientPoint point);
        inline void insert(GradientPointsIt it);
        inline void erase(GradientPointsIt it);
        inline size_t addPoint(GradientPoint point, \
            size_t index);
    };
}