#pragma once

#include <QWidget>
#include <QSize>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QRect>
#include <QPoint>
#include <QColor>
#include "../../Theme/Gradient/Gradient.cpp"

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
        inline void addPoint(bool after);
        void removePoint(void);

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