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
        Theme::Gradient &m_gradient;
        QGridLayout m_layoutMain;
        QLabel m_
        GradientStrip m_gradientStrip;
        QComboBox m_typeCombo;
        QDoubleSpinBox m_angleSpin;
        QComboBox m_centerCombo;
        QPushButton m_addButtonLeft;
        QPushButton m_addButtonRight;
        QPushButton m_removeButton;
        QPushButton m_colorButton;
        QPushButton m_isGoToNextPointButton;
        QLabel m_previewLabel;
        QLabel m_angleLabel;
        QLabel m_centerLabel;
        QMenu m_menu;
        QWidgetAction m_colorPickerWidgetAction;
        SelecterColor::ColorPicker m_colorPicker;
    public:
        explicit GradientEditor(Theme::Gradient &gradient, \
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
