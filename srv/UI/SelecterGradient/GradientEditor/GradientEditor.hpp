#pragma once

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QLinearGradient>
#include <QPainter>
#include <QHBoxLayout>
#include <QWidget>
#include <QGradient>

#include <functional>
#include <memory>

#include "../GradientStrip/GradientStrip.cpp"
#include "../../SelecterColor/ColorPicker/ColorPicker.cpp"

class QComboBox;
class QDoubleSpinBox;
class QCheckBox;
class QPushButton;
class QLabel;

namespace SelecterColor {
    class ColorPicker;
}

namespace SelecterGradient {
    class GradientStrip;
}

namespace SelecterGradient {
    class GradientEditor;
}

using GradientChangedCallback = std::function<void()>;

namespace SelecterGradient {
    class GradientEditor : public QWidget {
    private:
        Gradient &_gradient;
        GradientStrip *_gradientStrip{nullptr};
        QComboBox *_typeCombo{nullptr};
        QDoubleSpinBox *_angleSpin{nullptr};
        QComboBox *_centerCombo{nullptr};
        QCheckBox *_rotateCheck{nullptr};
        QPushButton *_addButton{nullptr};
        QPushButton *_removeButton{nullptr};
        QPushButton *_colorButton{nullptr};
        QLabel *_previewLabel{nullptr};
        QLabel *_angleLabel{nullptr};
        QLabel *_centerLabel{nullptr};

        QColor _currentColor;
        GradientChangedCallback _gradientChangedCallback;
    public:
        explicit GradientEditor(Gradient &gradient, QWidget *parent = nullptr);

        void setGradientChangedCallback(GradientChangedCallback callback);

        QGradientStops gradientStops() const;
        QGradient::Type gradientType() const;
        qreal angle() const;
        bool rotateWithShape() const;

    private:
        void addPoint();
        void removePoint();
        void updateColor(const QColor &color);
        void updateGradient();
        void setupUI();
        void updateColorButton(const QColor &color);
    };
}