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

#include "UI/SelecterGradient/GradientStrip/GradientStrip.hpp"
#include "UI/SelecterColor/ColorPicker/ColorPicker.hpp"

using GradientChangedCallback = std::function<void()>;

namespace SelecterGradient {
    class GradientEditor : public QWidget {
    private:
        QGradient &_qgradient;
        Theme::Gradient &_gradient;
        GradientStrip *_gradientStrip{nullptr};
        QComboBox *_typeCombo{nullptr};
        QDoubleSpinBox *_angleSpin{nullptr};
        QComboBox *_centerCombo{nullptr};
        QPushButton *_addButtonLeft{nullptr};
        QPushButton *_addButtonRight{nullptr};
        QPushButton *_removeButton{nullptr};
        QPushButton *_colorButton{nullptr};
        QPushButton *_isGoToNextPointButton{nullptr};
        QLabel *_previewLabel{nullptr};
        QLabel *_angleLabel{nullptr};
        QLabel *_centerLabel{nullptr};
        QMenu *_menu{nullptr};
        QWidgetAction *_colorPickerWidgetAction{nullptr};
        SelecterColor::ColorPicker *_colorPicker{nullptr};
        QColor *_currentColor;
    public:
        explicit GradientEditor(QGradient &qgradient, \
                Theme::Gradient *gradient, \
                QWidget *parent = nullptr);
        QGradient::Type gradientType() const;
        qreal angle() const;
        void addPointAfter(void);
        void addPointBefore(void);
        void setIsGoToNextPoint(bool newIsGoToNextPoint);
        bool getIsGoToNextPoint(void);

    protected:
        void paintEvent(QPaintEvent *event) override;
    private:
        void removePoint();
        void updateColor(const QColor &color);
        void updateGradient();
        void updateColorButton(const QColor &color);
    };
}
