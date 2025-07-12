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
    class GradientPoint {
    private:
        qreal _position;
        QColor _color;
        bool _isSelected;
    public:
        inline explicit GradientPoint( \
            qreal position, QColor color, \
            bool isSelected \
        ) : _position{position}, _color{color}, \
        _isSelected{isSelected} { return; }

        bool getIsSelected(void) const {
            return _isSelected;
        }
        void setIsSelected(bool isSelected) {
            _isSelected = isSelected;
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

    using GradientPoints = std::vector<GradientPoint>;
    class Gradient {
    private:
        GradientPoints _gradient{};
        QGradient::Type _type{QGradient::LinearGradient};
        QGradient::Spread _spread{QGradient::PadSpread};
        QGradient::CoordinateMode _mode{QGradient::LogicalMode};
    public:
        inline explicit Gradient(QGradient::Type type = QGradient::LinearGradient, \
            QGradient::Spread spread = QGradient::PadSpread, \
            QGradient::CoordinateMode mode = QGradient::LogicalMode) \
            : _gradient{}, _type{type}, _spread{spread}, _mode{mode} \
            {return;}
        inline QGradient::Type getType(void) {
            return _type;
        }
        inline void setType(QGradient::Type type) {
            _type = type; return;
        }
        inline QGradient::Spread getSpread(void) {
            return _spread;
        }
        inline void setSpread(QGradient::Spread spread) {
            _spread = spread; return;
        }
        inline QGradient::CoordinateMode getMode(void) {
            return _mode;
        }
        inline void setMode(QGradient::CoordinateMode mode) {
            _mode = mode; return;
        }
        inline void setPointAt(qreal position, QColor color, bool isSelection) {
            _gradient.emplace_back(position, color, isSelection); return;
        }
        inline void setPointAt(GradientPoint point) {
            _gradient.push_back(point); return;
        }
        inline Gradient &operator<<(GradientPoint point) {
            _gradient.push_back(point); return *this;
        }
        inline GradientPoint operator[](size_t index) {
            return _gradient[index];
        }
        inline size_t size(void) {
            return _gradient.size();
        }
        inline std::vector<GradientPoint>::iterator begin(void) {
            return _gradient.begin();
        }
        inline std::vector<GradientPoint>::iterator end(void) {
            return _gradient.end();
        }
    };

    using StopSelectedCallback = std::function<void(int)>;
    using StopsChangedCallback = std::function<void()>;

    class GradientStrip : public QWidget {
    private:
        Gradient &_gradient;
        int _selectedIndex = -1;
        bool _dragging = false;
        int _dragStartX = 0;
        qreal _dragStartPos = 0.0;
        
        StopSelectedCallback _stopSelectedCallback;
        StopsChangedCallback _stopsChangedCallback;
    public:

        explicit GradientStrip(Gradient &gradient, QWidget *parent = nullptr);
        
        QSize sizeHint() const override;
        QGradientStops gradientStops() const;
        void setGradientStops(const QGradientStops &stops);
        int getSelectedIndex() const;
        
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