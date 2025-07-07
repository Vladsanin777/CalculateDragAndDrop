#include "alphaslider.h"

AlphaSlider::AlphaSlider(int beginValue, \
    int endValue, int defaultValue, QWidget *parent)
    : QSlider(parent) {
    setOrientation(Qt::Vertical);
    setRange(beginValue, endValue);
    setValue(defaultValue);
    setMinimumSize(40, 100);
}

void AlphaSlider::setBaseColor(const QColor &color) {
    // Обновляем только если цвет действительно изменился
    if (baseColor != color) {
        baseColor = color;
        backgroundDirty = true;
        update();
    }
}

void AlphaSlider::drawCheckerPattern() {
    const int patternSize = 11; // 5 + 6 = 11
    checkerPattern = QImage(patternSize, patternSize, QImage::Format_RGB32);
    checkerPattern.fill(Qt::white); // Белый фон по умолчанию
    
    QPainter painter(&checkerPattern);
    painter.setPen(Qt::NoPen);
    
    // Чистые непрозрачные цвета
    const QColor lightGray(55, 55, 55); // Светло-серый
    const QColor darkGray(20, 20, 20);  // Темно-серый
    
    // Четные позиции (5x5)
    painter.fillRect(0, 0, 5, 5, lightGray);   // Левый верх
    painter.fillRect(5, 5, 6, 6, lightGray);    // Правый низ
    
    // Нечетные позиции (6x6)
    painter.fillRect(5, 0, 6, 5, darkGray);    // Правый верх
    painter.fillRect(0, 5, 5, 6, darkGray);   // Левый низ
}

void AlphaSlider::updateBackground() {
    if (backgroundImage.size() != size()) {
        backgroundImage = QImage(size(), QImage::Format_ARGB32);
    }
    
    if (checkerPattern.isNull()) {
        drawCheckerPattern();
    }
    
    QPainter painter(&backgroundImage);
    
    // Заливаем фон шахматным паттерном
    for (int y = 0; y < height(); y += checkerPattern.height()) {
        for (int x = 0; x < width(); x += checkerPattern.width()) {
            painter.drawImage(x, y, checkerPattern);
        }
    }
    
    // Рисуем градиент альфа-канала поверх паттерна
    QLinearGradient gradient(0, 0, 0, height());
    QColor opaqueColor = baseColor;
    opaqueColor.setAlpha(255);
    gradient.setColorAt(0.0, opaqueColor);
    gradient.setColorAt(1.0, Qt::transparent);
    
    painter.fillRect(0, 0, width(), height(), gradient);
}

void AlphaSlider::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    
    // Обновляем фон при необходимости
    if (backgroundDirty || backgroundImage.size() != size()) {
        updateBackground();
        backgroundDirty = false;
    }
    
    // Рисуем подготовленный фон
    painter.drawImage(5, 3, backgroundImage.copy(1, 0, width() - 9, height()-6));
    
    // Рассчитываем позицию белого прямоугольника
    qreal ratio = static_cast<qreal>(value()) / (maximum() - minimum());
    int yPos = (1 - ratio) * height();
    yPos = qBound(0, yPos - handleHeight/2, height() - handleHeight);
    
    // Рисуем ползунок на всю ширину виджета (включая отступы)
    QRect handleRect(1, yPos+1, width()-3, handleHeight-3);

    // 1. Рисуем основной белый контур с прозрачностью
    painter.setPen(QPen(QColor(255, 255, 255, 204), 1)); // 255*0.8=204
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(handleRect);

    // 2. Рисуем дополнительный чёрный контур без углов
    painter.setPen(QPen(QColor(0, 0, 0, 51), 1)); // 0.2*255=51

    int x = handleRect.x();
    int y = handleRect.y();
    int w = handleRect.width();
    int h = handleRect.height();

    // Верхняя линия (без углов)
    painter.drawLine(x, y-1,     x + w, y-1);
    // Нижняя линия (без углов)
    painter.drawLine(x, y + h + 1, x + w, y + h + 1);
    // Левая линия (без углов)
    painter.drawLine(x-1,     y, x-1,     y + h);
    // Правая линия (без углов)
    painter.drawLine(x + w + 1, y, x + w + 1, y + h);
}

void AlphaSlider::mousePressEvent(QMouseEvent *event) {
    // Преобразуем позицию Y в значение альфа
    qreal ratio = static_cast<qreal>(event->pos().y()) / height();
    int alphaValue = (1 - ratio) * maximum();
    setValue(qBound(minimum(), alphaValue, maximum()));
}

void AlphaSlider::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        qreal ratio = static_cast<qreal>(event->pos().y()) / height();
        int alphaValue = (1 - ratio) * maximum();
        setValue(qBound(minimum(), alphaValue, maximum()));
    }
}

void AlphaSlider::resizeEvent(QResizeEvent *event) {
    QSlider::resizeEvent(event);
    backgroundDirty = true;
}