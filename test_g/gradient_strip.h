#pragma once

#include <QWidget>
#include <QGradient>
#include <functional>
#include <vector>

struct GradientStop {
    qreal position;
    QColor color;
    bool isEndPoint;
    bool isSelected;
};

class GradientStrip : public QWidget {
public:
    using StopSelectedCallback = std::function<void(int)>;
    using StopsChangedCallback = std::function<void()>;

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

    std::vector<GradientStop> stops;
    int selectedIndex = -1;
    bool dragging = false;
    int dragStartX = 0;
    qreal dragStartPos = 0.0;
    
    StopSelectedCallback stopSelectedCallback;
    StopsChangedCallback stopsChangedCallback;
};