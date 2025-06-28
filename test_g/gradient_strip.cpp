#include "gradient_strip.h"
#include <QPainter>
#include <QMouseEvent>
#include <algorithm>

const int STRIP_HEIGHT = 30;
const int HANDLE_SIZE = 12;
const int HANDLE_RADIUS = 6;

GradientStrip::GradientStrip(QWidget *parent) 
    : QWidget(parent), selectedIndex(-1), dragging(false) {
    setMinimumHeight(50);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    
    // Начальные точки
    stops.push_back({0.0, Qt::red, true, false});
    stops.push_back({1.0, Qt::blue, true, false});
}

QSize GradientStrip::sizeHint() const {
    return QSize(300, STRIP_HEIGHT + HANDLE_SIZE + 5);
}

QGradientStops GradientStrip::gradientStops() const {
    QGradientStops result;
    for (const auto &stop : stops) {
        result << qMakePair(stop.position, stop.color);
    }
    return result;
}

void GradientStrip::setGradientStops(const QGradientStops &newStops) {
    stops.clear();
    for (int i = 0; i < newStops.size(); ++i) {
        stops.push_back({
            newStops[i].first,
            newStops[i].second,
            i == 0 || i == newStops.size() - 1,
            false
        });
    }
    
    if (selectedIndex >= static_cast<int>(stops.size())) {
        selectedIndex = -1;
    } else if (selectedIndex >= 0) {
        stops[selectedIndex].isSelected = true;
    }
    
    update();
    if (stopsChangedCallback) stopsChangedCallback();
}

int GradientStrip::selectedStop() const {
    return selectedIndex;
}

void GradientStrip::setStopSelectedCallback(StopSelectedCallback callback) {
    stopSelectedCallback = callback;
}

void GradientStrip::setStopsChangedCallback(StopsChangedCallback callback) {
    stopsChangedCallback = callback;
}

void GradientStrip::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Отрисовка градиента
    QLinearGradient grad(0, 0, width(), 0);
    for (const auto &stop : stops) {
        grad.setColorAt(stop.position, stop.color);
    }
    
    painter.fillRect(0, 0, width(), STRIP_HEIGHT, grad);
    
    // Отрисовка контрольных точек
    for (size_t i = 0; i < stops.size(); ++i) {
        const auto &stop = stops[i];
        int x = stop.position * width();
        int y = STRIP_HEIGHT + HANDLE_SIZE / 2;
        
        QRect rect = stopRect(static_cast<int>(i));
        
        // Выбор формы
        if (stop.isEndPoint) {
            painter.setBrush(stop.color);
            painter.setPen(stop.isSelected ? Qt::white : Qt::black);
            painter.drawRect(rect);
        } else {
            painter.setBrush(stop.color);
            painter.setPen(stop.isSelected ? Qt::white : Qt::black);
            painter.drawEllipse(rect);
        }
    }
}

QRect GradientStrip::stopRect(int index) const {
    if (index < 0 || index >= static_cast<int>(stops.size())) 
        return QRect();
    
    int x = stops[index].position * width() - HANDLE_SIZE / 2;
    int y = STRIP_HEIGHT;
    return QRect(x, y, HANDLE_SIZE, HANDLE_SIZE);
}

int GradientStrip::stopAtPosition(const QPoint &pos) const {
    for (int i = 0; i < static_cast<int>(stops.size()); ++i) {
        if (stopRect(i).contains(pos)) {
            return i;
        }
    }
    return -1;
}

void GradientStrip::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        int index = stopAtPosition(event->pos());
        if (index >= 0) {
            // Сброс предыдущего выбора
            if (selectedIndex >= 0 && selectedIndex < static_cast<int>(stops.size())) {
                stops[selectedIndex].isSelected = false;
            }
            
            // Установка нового выбора
            selectedIndex = index;
            stops[selectedIndex].isSelected = true;
            
            // Начало перетаскивания (только для средних точек)
            if (!stops[selectedIndex].isEndPoint) {
                dragging = true;
                dragStartX = event->pos().x();
                dragStartPos = stops[selectedIndex].position;
            }
            
            if (stopSelectedCallback) stopSelectedCallback(selectedIndex);
            update();
        }
    }
}

void GradientStrip::mouseMoveEvent(QMouseEvent *event) {
    if (dragging && selectedIndex >= 0) {
        int dx = event->pos().x() - dragStartX;
        qreal newPos = dragStartPos + static_cast<qreal>(dx) / width();
        
        // Ограничение позиции между соседними точками
        newPos = qBound(
            selectedIndex > 0 ? stops[selectedIndex-1].position + 0.01 : 0.0,
            newPos,
            selectedIndex < static_cast<int>(stops.size())-1 ? stops[selectedIndex+1].position - 0.01 : 1.0
        );
        
        stops[selectedIndex].position = newPos;
        update();
        if (stopsChangedCallback) stopsChangedCallback();
    }
}

void GradientStrip::mouseReleaseEvent(QMouseEvent *) {
    dragging = false;
}