#pragma once
#include <QColor>

namespace Theme {
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
    };
}
