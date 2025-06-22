#pragma once

class AlphaSlider : public QSlider {
public:
    explicit AlphaSlider(int beginValue, \
        int endValue, int defaultValue, \
        QWidget *parent = nullptr);
    
    void setBaseColor(const QColor &color);
    void updateBackground();
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    
private:
    void drawCheckerPattern();
    
    QColor baseColor = Qt::red;  // Базовый цвет (без альфа)
    QImage backgroundImage;      // Кэшированное фоновое изображение
    QImage checkerPattern;       // Шахматный паттерн
    bool backgroundDirty = true;
    const int checkerSize = 5;   // Размер клетки шахматного паттерна
    const int handleHeight = 7; // Высота белого прямоугольника-ползунка
};