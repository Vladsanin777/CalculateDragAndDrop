#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QPoint>
#include <QImage>
#include <function>

class QWidget;
class QColor;
class QPaintEvent;
class QMouseEvent;
class QResizeEvent;
class QPoint;
class QImage;

namespace SelecterColor {
    class ColorArea2D;
}

namespace SelecterColor {
    class ColorArea2D : public QWidget {
    private:
        qreal baseHue = 0.0;     // Основной цвет (устанавливается слайдером)
        qreal saturation = 1.0;  // Насыщенность (X-координата в области)
        qreal value = 1.0;       // Яркость (Y-координата в области)
        QImage background;
        bool backgroundDirty = true;
        std::function<void(qreal, qreal, qreal)> colorChangedCallback;
    public:
        explicit ColorArea2D(QWidget *parent = nullptr);
        void setBaseHue(qreal hue);
        void setSaturationValue(qreal sat, qreal val);
        qreal getBaseHue() const { return baseHue; };
        qreal getSaturation() const { return saturation; }
        qreal getValue() const { return value; }
        
        void setColorChangedCallback(std::function<void(qreal, qreal, qreal)> callback);

        QColor getCurrentColor() const;
    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void resizeEvent(QResizeEvent *event) override;

    private:
        void updateBackground();
        void selectColorAt(const QPoint &pos);

    };
}