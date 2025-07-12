#pragma once

#include "GradientStrip.hpp"

const int STRIP_HEIGHT = 24;  // Увеличена высота для размещения точек
const int POINT_EXTRA = 3;    // Выступ точек сверху и снизу
const int HANDLE_SIZE = 16;   // Размер квадратной части точки
const int TOTAL_HEIGHT = STRIP_HEIGHT + 2 * POINT_EXTRA; // Общая высота с учетом выступа

namespace SelecterGradient {
    GradientStrip::GradientStrip(Gradient &gradient, QWidget *parent) 
        : QWidget(parent), _gradient{gradient}, _selectedIndex(-1), \
        _dragging(false) {
        setMinimumHeight(TOTAL_HEIGHT);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }

    QSize GradientStrip::sizeHint() const {
        return QSize(300, TOTAL_HEIGHT);
    }

    /*
    QGradientStops GradientStrip::gradientStops() const {
        QGradientStops result;
        for (const auto &stop : _stops) {
            result << qMakePair(stop.getPosition(), stop.getColor());
        }
        return result;
    }

    void GradientStrip::setGradientStops(const QGradientStops &newStops) {
        _stops.clear();
        for (int i = 0; i < newStops.size(); ++i) {
            _stops.emplace_back(
                newStops[i].first,
                newStops[i].second,
                i == 0 || i == newStops.size() - 1,
                false
            );
        }
        
        if (_selectedIndex >= static_cast<int>(_stops.size())) {
            _selectedIndex = -1;
        } else if (_selectedIndex >= 0) {
            _stops[_selectedIndex].setIsSelected(true);
        }
        
        update();
        if (_stopsChangedCallback) _stopsChangedCallback();
    }

    int GradientStrip::getSelectedPoint() const {
        return _selectedIndex;
    }

    void GradientStrip::setStopSelectedCallback(StopSelectedCallback callback) {
        _stopSelectedCallback = callback;
    }

    void GradientStrip::setStopsChangedCallback(StopsChangedCallback callback) {
        _stopsChangedCallback = callback;
    }
    */

    void GradientStrip::paintEvent(QPaintEvent *) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        
        // Отрисовка градиента (с отступом для точек)
        QLinearGradient gradient(0, POINT_EXTRA, width(), POINT_EXTRA);
        for (const auto &point : _gradient) {
            gradient.setColorAt(point.getPosition(), point.getColor());
        }
        
        // Рисуем полосу градиента с отступами сверху и снизу
        painter.fillRect(0, POINT_EXTRA, width(), STRIP_HEIGHT, gradient);

        size_t size {_gradient.size()};
        // Отрисовка контрольных точек по центру градиента
        for (size_t i = 0; i < size; ++i) {
            const auto &point = _gradient[i];
            int x = point.getPosition() * width() - HANDLE_SIZE / 2;
            int y = TOTAL_HEIGHT / 2 - HANDLE_SIZE / 2;
            
            QRect rect(x, y, HANDLE_SIZE, HANDLE_SIZE);
            
            /*
            // Выбор формы
            if (stop.getIsEndPoint()) {
                // Прямоугольник с выступами
                QRect extendedRect(
                    x - POINT_EXTRA, 
                    y - POINT_EXTRA, 
                    HANDLE_SIZE + 2 * POINT_EXTRA, 
                    HANDLE_SIZE + 2 * POINT_EXTRA
                );
                
                painter.setBrush(stop.getColor());
                painter.setPen(stop.getIsSelected() ? Qt::white : Qt::black);
                painter.drawRect(extendedRect);
            } else {
                */
                // Круг
                painter.setBrush(point.getColor());
                painter.setPen(point.getIsSelected() ? Qt::white : Qt::black);
                painter.drawEllipse(rect);
            //}
        }
    }

    QRect GradientStrip::stopRect(int index) const {
        if (index < 0 || index >= static_cast<int>(_gradient.size())) 
            return QRect();
        
        const auto& point = _gradient[index];
        int x = point.getPosition() * width() - HANDLE_SIZE / 2;
        int y = TOTAL_HEIGHT / 2 - HANDLE_SIZE / 2;
        
        /*
        if (point.getIsEndPoint()) {
            // Прямоугольник с выступами
            return QRect(
                x - POINT_EXTRA, 
                y - POINT_EXTRA, 
                HANDLE_SIZE + 2 * POINT_EXTRA, 
                HANDLE_SIZE + 2 * POINT_EXTRA
            );
        }
        */
        // Круг
        return QRect(x, y, HANDLE_SIZE, HANDLE_SIZE);
    }

    int GradientStrip::stopAtPosition(const QPoint &pos) const {
        for (size_t i = 0; i < _gradient.size(); ++i) {
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
                if (_selectedIndex >= 0 && _selectedIndex < static_cast<int>(_gradient.size())) {
                    _gradient[_selectedIndex].setIsSelected(false);
                }
                
                // Установка нового выбора
                _selectedIndex = index;
                _gradient[_selectedIndex].setIsSelected(true);
                
                /*
                // Начало перетаскивания (только для средних точек)
                if (!_gradient[_selectedIndex].getIsEndPoint()) {
                    _dragging = true;
                    _dragStartX = event->pos().x();
                    _dragStartPos = _stops[_selectedIndex].getPosition();
                }
                */
                if (_stopSelectedCallback) _stopSelectedCallback(_selectedIndex);
                update();
            }
        }
    }

    void GradientStrip::mouseMoveEvent(QMouseEvent *event) {
        if (_dragging && _selectedIndex >= 0) {
            int dx = event->pos().x() - _dragStartX;
            qreal newPos = _dragStartPos + static_cast<qreal>(dx) / width();
            
            // Убираем ограничения - можно перемещать по всей длине
            newPos = qBound(0.0, newPos, 1.0);
            
            // Проверка на пересечение с другими точками
            for (size_t i = 0; i < _gradient.size(); ++i) {
                if (i != _selectedIndex && qAbs(_gradient[i].getPosition() - newPos) < 0.01) {
                    // Небольшое смещение для визуального разделения
                    if (newPos > _gradient[i].getPosition()) newPos = _gradient[i].getPosition() + 0.01;
                    else newPos = _gradient[i].getPosition() - 0.01;
                    newPos = qBound(0.0, newPos, 1.0);
                    break;
                }
            }
            
            _gradient[_selectedIndex].setPosition(newPos);
            update();
            if (_stopsChangedCallback) _stopsChangedCallback();
        }
    }


    void GradientStrip::mouseReleaseEvent(QMouseEvent *) {
        if (_dragging) {
            _dragging = false;
            
            /*
            // Пересортировка точек при отпускании
            if (!_stops.empty() && _stops.size() > 2) {
                // Сохраняем крайние точки
                GradientStop firstStop = _stops.front();
                GradientStop lastStop = _stops.back();
                
                // Копируем средние точки
                GradientPoints middleStops;
                for (size_t i = 1; i < _stops.size() - 1; ++i) {
                    middleStops.push_back(_stops[i]);
                }
                
                // Сортируем средние точки по позиции
                std::sort(middleStops.begin(), middleStops.end(), 
                    [](const GradientStop& a, const GradientStop& b) {
                        return a.getPosition() < b.getPosition();
                    });
                
                // Собираем новый список
                std::vector<GradientStop> newStops;
                newStops.push_back(firstStop);
                for (auto& stop : middleStops) {
                    newStops.push_back(stop);
                }
                newStops.push_back(lastStop);
                
                // Обновляем список
                _stops = newStops;
                */
                
                // Обновляем выбранный индекс
                for (int i = 0; i < static_cast<int>(_gradient.size()); ++i) {
                    if (_gradient[i].getIsSelected()) {
                        _selectedIndex = i;
                        break;
                    }
                }
                
                update();
                if (_stopsChangedCallback) _stopsChangedCallback();
            //}
        }
    }
}