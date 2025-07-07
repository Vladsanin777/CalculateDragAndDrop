#pragma once

#include <QWidget>
#include <QGradient>
#include <functional>
#include <memory>
#include "GradientStrip.cpp"
#include "../../Selector/ColorPicker.cpp"
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QLinearGradient>
#include <QPainter>
#include <QHBoxLayout>

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
    QComboBox *centerCombo;
    QCheckBox *rotateCheck;
    QPushButton *addButton;
    QPushButton *removeButton;
    QPushButton *colorButton;
    QLabel *previewLabel;
    QLabel *angleLabel;
    QLabel *centerLabel;

    QColor currentColor;
    GradientChangedCallback gradientChangedCallback;
};