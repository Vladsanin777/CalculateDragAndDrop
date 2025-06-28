#pragma once

#include <QWidget>
#include <QGradient>
#include <functional>
#include <memory>

class QComboBox;
class QDoubleSpinBox;
class QCheckBox;
class QPushButton;
class GradientStrip;
class QLabel;

class GradientEditor : public QWidget {
public:
    explicit GradientEditor(QWidget *parent = nullptr);

    using GradientChangedCallback = std::function<void()>;
    void setGradientChangedCallback(GradientChangedCallback callback);

    QGradientStops gradientStops() const;
    QGradient::Type gradientType() const;
    qreal angle() const;
    bool rotateWithShape() const;

private:
    void addStop();
    void removeStop();
    void updateColor(const QColor &color);
    void updateGradient();
    void setupUI();
    void updateColorButton(const QColor &color);

    GradientStrip *gradientStrip;
    QComboBox *typeCombo;
    QDoubleSpinBox *angleSpin;
    QCheckBox *rotateCheck;
    QPushButton *addButton;
    QPushButton *removeButton;
    QPushButton *colorButton;
    QLabel *previewLabel;
    
    QColor currentColor;
    GradientChangedCallback gradientChangedCallback;
};