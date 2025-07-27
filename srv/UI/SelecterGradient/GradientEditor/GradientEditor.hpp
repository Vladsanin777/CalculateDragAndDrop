#pragma once

#include <QComboBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QLinearGradient>
#include <QMenu>
#include <QWidgetAction>
#include <QPainter>
#include <QHBoxLayout>
#include <QWidget>
#include <QGradient>

#include <functional>
#include <memory>

#include "UI/SelecterGradient/GradientStrip/GradientStrip.cpp"
#include "UI/SelecterColor/ColorPicker/ColorPicker.cpp"

class QComboBox;
class QDoubleSpinBox;
class QCheckBox;
class QPushButton;
class QLabel;
class QWidgetAction;
class QMenu;

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
        Theme::Gradient &_gradient;
        GradientStrip *_gradientStrip{nullptr};
        QComboBox *_typeCombo{nullptr};
        QDoubleSpinBox *_angleSpin{nullptr};
        QComboBox *_centerCombo{nullptr};
        QPushButton *_addButtonLeft{nullptr};
        QPushButton *_addButtonRight{nullptr};
        QPushButton *_removeButton{nullptr};
        QPushButton *_colorButton{nullptr};
        QLabel *_previewLabel{nullptr};
        QLabel *_angleLabel{nullptr};
        QLabel *_centerLabel{nullptr};
        QMenu *_menu{nullptr};
        QWidgetAction *_colorPickerWidgetAction{nullptr};
        SelecterColor::ColorPicker *_colorPicker{nullptr};
        QColor *_currentColor;
    public:
        explicit GradientEditor(Theme::Gradient &gradient, QWidget *parent = nullptr);

        //void setGradientChangedCallback(GradientChangedCallback callback);

        QGradient::Type gradientType() const;
        qreal angle() const;
        //bool rotateWithShape() const;
        inline void addPoint(bool after);

    private:
        void removePoint();
        void updateColor(const QColor &color);
        void updateGradient();
        void setupUI();
        void updateColorButton(const QColor &color);
    };
}