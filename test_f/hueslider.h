#pragma once


class HueSlider : public QSlider {
public:
    explicit HueSlider(int beginValue, \
        int endValue, int defaultValue, \
        QWidget *parent = nullptr);
    
    void updateGradient();
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    
private:
    QImage gradientImage;
    bool gradientDirty = true;
    const int handleHeight = 7; // Высота белого прямоугольника-ползунка
};