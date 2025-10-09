#include <iostream>
#include <algorithm>
#include "UI/SelecterGradient/GradientStrip/GradientStrip.hpp"
#include <QPainter>

const int STRIP_HEIGHT = 24;  // Увеличена высота для размещения точек
const int POINT_EXTRA = 3;    // Выступ точек сверху и снизу
const int HANDLE_SIZE = 16;   // Размер квадратной части точки
const int TOTAL_HEIGHT = STRIP_HEIGHT + 2 * POINT_EXTRA; // Общая высота с учетом выступа

namespace SelecterGradient {
    GradientStrip::GradientStrip(Theme::Gradient &gradient, \
        QColor *&currentColor, QWidget *parent) 
        : QWidget{parent}, _gradient{gradient}, _selectedIndex(0), \
        _dragging(false), _currentColor{currentColor}, \
        _isGoToNextPoint{true} {
        setMinimumHeight(TOTAL_HEIGHT);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        setSelectedIndex(0);
    }

    QSize GradientStrip::sizeHint(void) const {
        return QSize(300, TOTAL_HEIGHT);
    }

    size_t GradientStrip::getSelectedIndex(void) const {
        return _selectedIndex;
    }

    size_t GradientStrip::size(void) {
        return _gradient.size();
    }

    void GradientStrip::setSelectedIndex(size_t selectedIndex) {
        _selectedIndex = selectedIndex; 
        _currentColor = &_gradient[selectedIndex].getColor();
        printf("jklsdk");
        update(); return;
    }
    void GradientStrip::addPointIndex(size_t index) {
        Theme::GradientPoint resultPoint{};
        std::cout << _gradient << std::endl;
        printf("index: %lu\n", index);
        if (index == 0) {
            Theme::GradientPoint &point { _gradient[index] };
            qreal pos { point.getPosition() / 2 };
            resultPoint.setColor(point.getColor());
            resultPoint.setPosition(pos);
        } else if (index == _gradient.size()) {

            printf("index %lu addPointIndex\n", index);
            Theme::GradientPoint &point { _gradient[index - 1] };
            qreal pos { (1.0 + point.getPosition()) / 2 };
            resultPoint.setColor(point.getColor());
            resultPoint.setPosition(pos);
        } else {
            Theme::GradientPoint &point0 {_gradient[index - 1]};
            Theme::GradientPoint &point1 {_gradient[index]};
            QColor& color0 {point0.getColor()};
            QColor& color1 {point1.getColor()};
            qreal &pos0 {point0.getPosition()};
            qreal &pos1 {point1.getPosition()};
            QColor resultColor {(color0.red() + color1.red()) >> 1, \
                (color0.green() + color1.green()) >> 1, \
                (color0.blue() + color1.blue()) >> 1};
            qreal resultPos { (pos1 + pos0) / 2 };
            resultPoint.setColor(resultColor);
            resultPoint.setPosition(resultPos);
        }
        _gradient.addPoint(resultPoint, index);
        update();
        return;
    }

    void GradientStrip::addPointAfter(void) {
        addPointIndex(getSelectedIndex() + 1);
        if (_isGoToNextPoint)
            setSelectedIndex(getSelectedIndex()+1);
        return;
    }

    bool GradientStrip::getIsGoToNextPoint(void) {
        return _isGoToNextPoint;
    }

    void GradientStrip::setIsGoToNextPoint(bool newIsGoToNextPoint) {
        _isGoToNextPoint = newIsGoToNextPoint;
        std::cout << "setIsGoToNextPoint: " << _isGoToNextPoint << std::endl;
        return;
    }
    void GradientStrip::addPointBefore(void) {
        addPointIndex(getSelectedIndex());
        if (!_isGoToNextPoint)
            setSelectedIndex(getSelectedIndex() + 1);
        return;
    }

    void GradientStrip::removePoint(void) {
        _gradient.erase(_gradient.begin() + _selectedIndex);
        if (_selectedIndex == size()) _selectedIndex--;
        return;
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
            const Theme::GradientPoint &point = _gradient[index];
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
                setSelectedIndex(index);
                
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

        if (!_dragging) return;

        int dx = event->pos().x() - _dragStartX;
        qreal newPos { qBound(0.0, _dragStartPos + \
                static_cast<qreal>(dx) / width(), 1.0) };

        _gradient[_selectedIndex].setPosition(newPos);

        if (_selectedIndex != 0 && _selectedIndex < \
                _gradient.size() - 1) {


            Theme::GradientPoint & pointSelect \
                { _gradient[_selectedIndex] }, \
                & pointPreced { _gradient[_selectedIndex - 1] }, \
                & pointNext { _gradient[_selectedIndex + 1] };

            if (newPos < pointPreced.getPosition()) {
                std::swap(pointSelect, pointPreced);
                _selectedIndex--;
            } else if (newPos > pointNext.getPosition()) {
                std::swap(pointSelect, pointNext);
                _selectedIndex++;
            }
        } else if (_selectedIndex == 0) {
            if (_gradient.size() < 2) { update(); return; }
            Theme::GradientPoint & pointSelect \
                { _gradient[_selectedIndex] }, \
                & pointNext { _gradient[_selectedIndex + 1] };
            if (newPos > pointNext.getPosition()) {
                std::swap(pointSelect, pointNext);
                _selectedIndex++;
            }
        } else {
            Theme::GradientPoint & pointSelect \
                { _gradient[_selectedIndex] }, \
                & pointPreced { _gradient[_selectedIndex - 1] };
            if (newPos < pointPreced.getPosition()) {
                std::swap(pointSelect, pointPreced);
                _selectedIndex--;
            }
        }
        update();
    }

    void GradientStrip::mouseReleaseEvent(QMouseEvent *) {
        if (_dragging) {
            _dragging = false;
            update();
        }
    }
}
