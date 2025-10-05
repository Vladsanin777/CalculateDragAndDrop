#pragma once

#include <QWidget>
#include <QSize>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QRect>
#include <QPoint>
#include <QColor>

#include "UI/Theme/Gradient/Gradient.hpp"

#include <functional>
#include <vector>

class QWidget;
class QSize;
class QPaintEvent;
class QMouseEvent;
class QRect;
class QPoint;
class QColor;

namespace Theme {
    class Gradient;
}

namespace SelecterGradient {
    class GradientStrip : public QWidget {
    private:
        Theme::Gradient &_gradient;
        size_t _selectedIndex = 0;
        bool _dragging = false;
        int _dragStartX = 0;
        qreal _dragStartPos = 0.0;
        QColor *&_currentColor;
        
        
        //StopSelectedCallback _stopSelectedCallback;
        //StopsChangedCallback _stopsChangedCallback;
    public:

        GradientStrip(Theme::Gradient &gradient, QColor *&_currentColor, \
                QWidget *parent = nullptr);
        
        QSize sizeHint(void) const override;
        size_t getSelectedIndex(void) const;
        void removePoint(void);
        void addPointBefore(void);
        void addPointAfter(void);

    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;

    private:
        void addPointIndex(size_t index);
        void setSelectedIndex(size_t selectedIndex);
        void updatePointPositions();
        QRect pointRect(int index) const;
        int pointAtPosition(const QPoint &pos) const;
    };
}
