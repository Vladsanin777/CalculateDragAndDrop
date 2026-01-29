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
        Theme::Gradient &m_gradient;
        size_t m_selectedIndex = 0;
        bool m_dragging = false;
        int m_dragStartX = 0;
        qreal m_dragStartPos = 0.0;
        
        //StopSelectedCallback _stopSelectedCallback;
        //StopsChangedCallback _stopsChangedCallback;
    public:

        GradientStrip(Theme::Gradient &gradient, QColor *&_currentColor, \
                QWidget *parent = nullptr);
        
        QSize sizeHint(void) const override;
        size_t getSelectedIndex(void) const;
        void setSelectedIndex(size_t selectedIndex);
        void removePoint(void);
        void addPointBefore(void);
        void addPointAfter(void);
        size_t size(void);

    protected:
        void paintEvent(QPaintEvent *event) override;
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;

    private:
        void addPointIndex(size_t index);
        void updatePointPositions();
        QRect pointRect(int index) const;
        int pointAtPosition(const QPoint &pos) const;
    slots:
        void update(void);
    };
}
