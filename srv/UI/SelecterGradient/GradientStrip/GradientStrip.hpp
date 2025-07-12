#pragma once

#include <QWidget>
#include <QSize>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QRect>
#include <QPoint>
#include <QColor>

#include <cstddef>
#include <functional>
#include <vector>

class QWidget;
class QSize;
class QPaintEvent;
class QMouseEvent;
class QRect;
class QPoint;
class QColor;

namespace SelecterGradient {
    class GradientPoint {
    private:
        qreal _position;
        QColor _color;
    public:
        inline GradientPoint( \
            qreal position, QColor color \
        ) : _position{position}, _color{color} \
        { return; }

        inline qreal &getPosition(void) {
            return _position;
        }
        inline qreal getPosition(void) const {
            return _position;
        }
        inline void setPosition(qreal position) {
            _position = position;
            return;
        }
        inline QColor &getColor(void) {
            return _color;
        }
        inline QColor getColor(void) const {
            return _color;
        }
        inline void setColor(QColor color) {
            _color = color;
            return;
        }
    };

    using GradientPoints = std::vector<GradientPoint>;
    using GradientPointsIt = GradientPoints::iterator;
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
        inline GradientPoint& operator[](size_t index) {
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
        inline void insert(GradientPointsIt it, \
            GradientPoint point) {
            _gradient.insert(it, point); return;
        }
        inline void insert(GradientPointsIt it) {
            _gradient.erase(it); return;
        }
        inline size_t addPoint(GradientPoint point, \
            size_t index) {
            insert(begin() + index, point);
            return index;
        }
        inline bool removePoint(size_t index) {
            if (index < size()) {
                _gradient.erase(begin()+index);
                return true;
            }
            return false;
        }
    };

    using StopSelectedCallback = std::function<void(int)>;
    using StopsChangedCallback = std::function<void()>;

    class GradientStrip : public QWidget {
    private:
        Gradient &_gradient;
        size_t _selectedIndex = 0;
        bool _dragging = false;
        int _dragStartX = 0;
        qreal _dragStartPos = 0.0;
        
        //StopSelectedCallback _stopSelectedCallback;
        //StopsChangedCallback _stopsChangedCallback;
    public:

        explicit GradientStrip(Gradient &gradient, QWidget *parent = nullptr);
        
        QSize sizeHint(void) const override;
        size_t getSelectedIndex(void) const;
        size_t addPoint(const GradientPoint point, bool after);
        void removePoint(size_t index);

    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;

    private:
        void setSelectedIndex(unsigned int selectedIndex);
        void updatePointPositions();
        QRect pointRect(int index) const;
        int pointAtPosition(const QPoint &pos) const;
    };
}