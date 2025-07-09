#pragma once

#include <QLinearGradient>
#include <QRadialGradient>
#include <QString>
#include <QColor>
#include <vector>
#include <utility>
#include <cmath>

class QLinearGradient;
class QRadialGradient;
class QString;
class QColor;

class GradientSettings {
public:
    enum class Type {
        Linear,
        Radial
    };

    // Конструкторы
    GradientSettings();
    GradientSettings(Type type, const std::vector<std::pair<double, QColor>>& stops, 
                     double angle = 90.0, const QString& center = "center",
                     bool rotateWithShape = false);

    // Методы доступа
    Type type() const;
    void setType(Type type);

    double angle() const;
    void setAngle(double angle);

    QString center() const;
    void setCenter(const QString& center);

    bool rotateWithShape() const;
    void setRotateWithShape(bool rotate);

    const std::vector<std::pair<double, QColor>>& stops() const;
    void setStops(const std::vector<std::pair<double, QColor>>& stops);
    void addStop(double position, const QColor& color);
    void removeStop(int index);

    // Преобразование в QGradient (для отрисовки)
    QGradient toQGradient(const QRectF& bounds) const;

private:
    Type m_type = Type::Linear;
    double m_angle = 90.0;
    QString m_center = "center";
    bool m_rotateWithShape = false;
    std::vector<std::pair<double, QColor>> m_stops;
};