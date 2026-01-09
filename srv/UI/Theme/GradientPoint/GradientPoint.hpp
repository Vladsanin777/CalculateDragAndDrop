#pragma once
#include <QColor>

std::ostream &operator<<(std::ostream &out, QColor &color);

namespace Theme {
    class GradientPoint;
    using GradientPoints = std::vector<GradientPoint>;
    using GradientPointsIt = GradientPoints::iterator;

    class GradientPoint {
    private:
        qreal _position;
        QColor _color;
    public:
        GradientPoint( \
            qreal position = 0.0, QColor color = QColor(0, 0, 0, 0) \
        );

        qreal &getPosition(void);
        qreal getPosition(void) const;
        void setPosition(qreal position);
        QColor &getColor(void);
        QColor getColor(void) const;
        void setColor(QColor color);
        friend std::ostream &operator<<(std::ostream &out, GradientPoint &point) {
            out << "color: " << point._color << "position: " << point._position;
            return out;
        }
    };
}
