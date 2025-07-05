#pragma once


class HueSlider;

class ColorArea2D : public QWidget {
public:
    explicit ColorArea2D(QWidget *parent = nullptr);
    void setBaseHue(qreal hue);
    void setSaturationValue(qreal sat, qreal val);
    qreal getBaseHue() const { return baseHue; };
    qreal getSaturation() const { return saturation; }
    qreal getValue() const { return value; }
    
    void setColorChangedCallback(std::function<void(qreal, qreal, qreal)> callback);

    QColor getCurrentColor() const;
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void updateBackground();
    void selectColorAt(const QPoint &pos);

    qreal baseHue = 0.0;     // Основной цвет (устанавливается слайдером)
    qreal saturation = 1.0;  // Насыщенность (X-координата в области)
    qreal value = 1.0;       // Яркость (Y-координата в области)
    QImage background;
    bool backgroundDirty = true;
    std::function<void(qreal, qreal, qreal)> colorChangedCallback;
};


class ColorPicker : public QWidget {
public:
    explicit ColorPicker(QColor color, QWidget *parent = nullptr);
    QColor color() const;
    void setColorChangedCallback(std::function<void(const QColor&)> callback);
    void updateAlphaSliderColor();
private:
    void handleHueChanged(int hueValue);
    void handleAreaChanged(qreal hue, qreal sat, qreal val);
    void handleAlphaChanged(int alphaValue);
    void updateColor();

    HueSlider *hueSlider;      // Вертикальный слайдер для цвета (Hue)
    ColorArea2D *colorArea;  // 2D-область для насыщенности и яркости
    AlphaSlider *alphaSlider;    // Вертикальный слайдер для прозрачности
    QColor currentColor;
    std::function<void(const QColor&)> colorChangedCallback;
};