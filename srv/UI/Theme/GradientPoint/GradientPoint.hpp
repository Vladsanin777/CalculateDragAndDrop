#include <QColor>

namespace Theme {
    class GradientPoint {
    private:
        qreal _position;
        QColor _color;
    public:
        inline GradientPoint( \
            qreal position, QColor color \
        );

        inline qreal &getPosition(void);
        inline qreal getPosition(void) const;
        inline void setPosition(qreal position);
        inline QColor &getColor(void);
        inline QColor getColor(void) const;
        inline void setColor(QColor color);
    };
}