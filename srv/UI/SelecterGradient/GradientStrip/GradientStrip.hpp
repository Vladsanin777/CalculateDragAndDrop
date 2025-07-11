#pragma once

#include <QWidget>
#include <QSize>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QRect>
#include <QPoint>
#include <QColor>

#include <functional>
#include <vector>

class QWidget;
class QSize;
class QPaintEvent;
class QMouseEvent;
class QReact;
class QPoint;
class QColor;

namespace SelecterGradient {
    class GradientStop {
    private:
        qreal _position;
        QColor _color;
        // 0bit - isSelected
        // 7bit - isEndPoint
        unsigned char _status;
    public:
        inline explicit GradientStop( \
            qreal position, QColor color, \
            bool isSelected, bool isEndPoint = false \
        ) : _position{position}, _color{color}, \
        _status{(unsigned char)0} {
            if (isEndPoint)
                _status |= 0b10000000;
            if (isSelected)
                _status |= 0b00000001;
            return;
        }
        bool getIsSelected(void) const {
            return _status & 0b00000001;
        }

        bool getIsEndPoint(void) const {
            return _status & 0b10000000;
        }
        void setIsSelected(bool isSelect) {
            if (isSelect)
                _status |= 0b00000001;
            else 
                _status &= 0b11111110;
            return;
        }
        qreal getPosition(void) const {
            return _position;
        }
        void setPosition(qreal position) {
            _position = position;
            return;
        }
        QColor getColor(void) const {
            return _color;
        }
        void setColor(QColor color) {
            _color = color;
            return;
        }
    };

    using StopSelectedCallback = std::function<void(int)>;
    using StopsChangedCallback = std::function<void()>;

    class GradientStrip : public QWidget {
    private:
        std::vector<GradientStop> _stops;
        int _selectedIndex = -1;
        bool _dragging = false;
        int _dragStartX = 0;
        qreal _dragStartPos = 0.0;
        
        StopSelectedCallback _stopSelectedCallback;
        StopsChangedCallback _stopsChangedCallback;
    public:

        explicit GradientStrip(QWidget *parent = nullptr);
        
        QSize sizeHint() const override;
        QGradientStops gradientStops() const;
        void setGradientStops(const QGradientStops &stops);
        int selectedStop() const;
        
        void setStopSelectedCallback(StopSelectedCallback callback);
        void setStopsChangedCallback(StopsChangedCallback callback);

    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;

    private:
        void updateStopPositions();
        QRect stopRect(int index) const;
        int stopAtPosition(const QPoint &pos) const;
    };
}