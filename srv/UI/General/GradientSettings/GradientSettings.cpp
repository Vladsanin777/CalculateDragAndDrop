#include "gradient_settings.h"
#include <QLinearGradient>
#include <QRadialGradient>
#include <cmath>

GradientSettings::GradientSettings() = default;

GradientSettings::GradientSettings(Type type, 
                                   const std::vector<std::pair<double, QColor>>& stops,
                                   double angle, 
                                   const QString& center,
                                   bool rotateWithShape)
    : m_type(type), m_stops(stops), m_angle(angle), 
      m_center(center), m_rotateWithShape(rotateWithShape) {}

GradientSettings::Type GradientSettings::type() const {
    return m_type;
}

void GradientSettings::setType(Type type) {
    m_type = type;
}

double GradientSettings::angle() const {
    return m_angle;
}

void GradientSettings::setAngle(double angle) {
    m_angle = angle;
}

QString GradientSettings::center() const {
    return m_center;
}

void GradientSettings::setCenter(const QString& center) {
    m_center = center;
}

bool GradientSettings::rotateWithShape() const {
    return m_rotateWithShape;
}

void GradientSettings::setRotateWithShape(bool rotate) {
    m_rotateWithShape = rotate;
}

const std::vector<std::pair<double, QColor>>& GradientSettings::stops() const {
    return m_stops;
}

void GradientSettings::setStops(const std::vector<std::pair<double, QColor>>& stops) {
    m_stops = stops;
}

void GradientSettings::addStop(double position, const QColor& color) {
    m_stops.push_back(std::make_pair(position, color));
    // Можно добавить сортировку при необходимости
}

void GradientSettings::removeStop(int index) {
    if (index >= 0 && index < static_cast<int>(m_stops.size())) {
        m_stops.erase(m_stops.begin() + index);
    }
}

QGradient GradientSettings::toQGradient(const QRectF& bounds) const {
    if (m_stops.empty()) {
        return QLinearGradient();
    }

    if (m_type == Type::Linear) {
        qreal radians = qDegreesToRadians(m_angle);
        qreal dx = std::cos(radians);
        qreal dy = std::sin(radians);

        QPointF center(bounds.center());
        qreal length = std::sqrt(bounds.width()*bounds.width() + bounds.height()*bounds.height()) / 2.0;

        QPointF start(center.x() - dx * length, center.y() - dy * length);
        QPointF end(center.x() + dx * length, center.y() + dy * length);

        QLinearGradient grad(start, end);
        for (const auto& stop : m_stops) {
            grad.setColorAt(stop.first, stop.second);
        }
        return grad;
    }
    else { // Radial
        QPointF centerPoint;
        qreal w = bounds.width();
        qreal h = bounds.height();

        if (m_center == "top-left") {
            centerPoint = bounds.topLeft();
        } else if (m_center == "top-right") {
            centerPoint = bounds.topRight();
        } else if (m_center == "bottom-left") {
            centerPoint = bounds.bottomLeft();
        } else if (m_center == "bottom-right") {
            centerPoint = bounds.bottomRight();
        } else { // "center"
            centerPoint = bounds.center();
        }

        // Рассчитываем максимальный радиус
        qreal maxRadius = 0;
        QPointF corners[] = {
            bounds.topLeft(),
            bounds.topRight(),
            bounds.bottomLeft(),
            bounds.bottomRight()
        };

        for (const auto& corner : corners) {
            qreal distance = QLineF(centerPoint, corner).length();
            if (distance > maxRadius) maxRadius = distance;
        }

        QRadialGradient grad(centerPoint, maxRadius);
        for (const auto& stop : m_stops) {
            grad.setColorAt(stop.first, stop.second);
        }
        return grad;
    }
}