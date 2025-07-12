#pragma once

#include "GradientStrip.hpp"

const int STRIP_HEIGHT = 24;  // Увеличена высота для размещения точек
const int POINT_EXTRA = 3;    // Выступ точек сверху и снизу
const int HANDLE_SIZE = 16;   // Размер квадратной части точки
const int TOTAL_HEIGHT = STRIP_HEIGHT + 2 * POINT_EXTRA; // Общая высота с учетом выступа

namespace SelecterGradient {
    GradientStrip::GradientStrip(Gradient &gradient, QWidget *parent) 
        : QWidget{parent}, _gradient{gradient}, _selectedIndex(-1), \
        _dragging(false) {
        setMinimumHeight(TOTAL_HEIGHT);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    }

    QSize GradientStrip::sizeHint(void) const {
        return QSize(300, TOTAL_HEIGHT);
    }

    size_t GradientStrip::getSelectedIndex(void) const {
        return _selectedIndex;
    }

    size_t GradientStrip::addPoint(const GradientPoint point, bool after) {
        size_t index {_gradient.addPoint(point, \
            after ? getSelectedIndex() + 1 : getSelectedIndex())};
        
        if (after) setSelectedIndex(index);
        return index;
    }

    void GradientStrip::removePoint(void) {
        size_t index {getSelectedIndex()};
        
        _gradient->erase(begin() + index);
    }

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
        for (size_t index {0}; index < size; ++index) {
            const GradientPoint &point = _gradient[index];
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
                painter.setPen(getSelectedIndex() == index ? Qt::white : Qt::black);
                painter.drawEllipse(rect);
            //}
        }
    }

    QRect GradientStrip::pointRect(int index) const {
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

    int GradientStrip::pointAtPosition(const QPoint &pos) const {
        for (size_t i = 0; i < _gradient.size(); ++i) {
            if (pointRect(i).contains(pos)) {
                return i;
            }
        }
        return -1;
    }

    void GradientStrip::mousePressEvent(QMouseEvent *event) {
        if (event->button() == Qt::LeftButton) {
            int index = pointAtPosition(event->pos());
            if (index >= 0) {
                
                // Установка нового выбора
                _selectedIndex = index;
                
                // Начало перетаскивания
                _dragging = true;
                _dragStartX = event->pos().x();
                _dragStartPos = _gradient[getSelectedIndex()].getPosition();

                // if (_stopSelectedCallback) _stopSelectedCallback(_selectedIndex);
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
            // if (_stopsChangedCallback) _stopsChangedCallback();
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
                
                // Обновляем выбранный индекс
                for (int i = 0; i < static_cast<int>(_gradient.size()); ++i) {
                    if (_gradient[i].getIsSelected()) {
                        _selectedIndex = i;
                        break;
                    }
                }
                */
                update();
                // if (_stopsChangedCallback) _stopsChangedCallback();
            //}
        }
    }
}