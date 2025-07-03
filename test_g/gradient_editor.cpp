#pragma once
#include "gradient_editor.h"
#include "gradient_strip.cpp"
#include "../test_f/colorpicker.cpp"
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QLinearGradient>
#include <QPainter>
#include <QHBoxLayout>

GradientEditor::GradientEditor(QWidget *parent) 
    : QWidget(parent), currentColor(Qt::black) {
    setupUI();
    
    // Инициализация градиента
    QGradientStops initialStops;
    initialStops << QGradientStop(0.0, Qt::red)
                 << QGradientStop(1.0, Qt::blue);
    gradientStrip->setGradientStops(initialStops);
    
    gradientStrip->setStopSelectedCallback([this](int index) {
        if (index >= 0 && index < gradientStrip->gradientStops().size()) {
            currentColor = gradientStrip->gradientStops().at(index).second;
            updateColorButton(currentColor);
            removeButton->setEnabled(index != 0 && 
                                    index != gradientStrip->gradientStops().size() - 1);
        }
    });
    
    gradientStrip->setStopsChangedCallback([this] {
        updateGradient();
    });
    show();
}

void GradientEditor::setGradientChangedCallback(GradientChangedCallback callback) {
    gradientChangedCallback = callback;
}

void GradientEditor::setupUI() {
    QGridLayout *layout = new QGridLayout(this);
    
    // Тип градиента
    typeCombo = new QComboBox(this);
    typeCombo->addItem(QObject::tr("Линейный"), QGradient::LinearGradient);
    typeCombo->addItem(QObject::tr("Радиальный"), QGradient::RadialGradient);
    layout->addWidget(new QLabel(QObject::tr("Тип:")), 0, 0);
    layout->addWidget(typeCombo, 0, 1);
    
    // Угол (сохраняем указатель на метку)
    angleLabel = new QLabel(QObject::tr("Угол:"), this);  // Сохраняем в angleLabel
    angleSpin = new QDoubleSpinBox(this);
    angleSpin->setRange(0, 360);
    angleSpin->setValue(90);
    angleSpin->setSuffix("°");
    layout->addWidget(angleLabel, 0, 2);
    layout->addWidget(angleSpin, 0, 3);
    
    // Предпросмотр
    previewLabel = new QLabel(this);
    previewLabel->setMinimumSize(100, 20);
    layout->addWidget(new QLabel(QObject::tr("Предварительный просмотр:")), 0, 4);
    layout->addWidget(previewLabel, 0, 5);
    
    // Полоса градиента
    gradientStrip = new GradientStrip(this);
    layout->addWidget(gradientStrip, 1, 0, 1, 6);
    
    // Кнопки управления
    addButton = new QPushButton(QObject::tr("Добавить"), this);
    removeButton = new QPushButton(QObject::tr("Удалить"), this);
    removeButton->setEnabled(false);
    
    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(addButton);
    buttonsLayout->addWidget(removeButton);
    layout->addLayout(buttonsLayout, 2, 0, 1, 2);
    
    // Кнопка выбора цвета
    colorButton = new QPushButton(this);
    colorButton->setFixedSize(30, 30);
    updateColorButton(Qt::black);
    layout->addWidget(new QLabel(QObject::tr("Цвет:")), 2, 2);
    layout->addWidget(colorButton, 2, 3);
    
    // Поворот с фигурой
    rotateCheck = new QCheckBox(QObject::tr("Повернуть вместе с фигурой"), this);
    layout->addWidget(rotateCheck, 3, 0, 1, 6);
    
    // Соединения
    connect(addButton, &QPushButton::clicked, this, [this] { addStop(); });
    connect(removeButton, &QPushButton::clicked, this, [this] { removeStop(); });
    connect(typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this] { 
                // Показывать/скрывать угол для радиального градиента
                bool isLinear = (typeCombo->currentData().toInt() == QGradient::LinearGradient);
                angleSpin->setVisible(isLinear);
                angleLabel->setVisible(isLinear);  // Используем сохраненный указатель
                updateGradient(); 
            });
    connect(angleSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, [this] { updateGradient(); });
    
    connect(colorButton, &QPushButton::clicked, this, [this] {
        auto menu = new ColorPicker(currentColor, this);
        //menu->setColorSelectedCallback([this](const QColor &color) { \
            updateColor(color); \
        });
        //menu->popup(colorButton->mapToGlobal(QPoint(0, colorButton->height())));
    });
    
    // Инициализация видимости угла
    bool isLinear = (typeCombo->currentData().toInt() == QGradient::LinearGradient);
    angleSpin->setVisible(isLinear);
    angleLabel->setVisible(isLinear);
}


void GradientEditor::addStop() {
    int index = gradientStrip->selectedStop();
    if (index < 0) return;
    
    auto stops = gradientStrip->gradientStops();
    if (index >= stops.size() - 1) return;
    
    qreal pos = (stops[index].first + stops[index+1].first) / 2;
    QColor color = QColor::fromRgbF(
        (stops[index].second.redF() + stops[index+1].second.redF()) / 2,
        (stops[index].second.greenF() + stops[index+1].second.greenF()) / 2,
        (stops[index].second.blueF() + stops[index+1].second.blueF()) / 2
    );
    
    stops.insert(stops.begin() + index + 1, qMakePair(pos, color));
    gradientStrip->setGradientStops(stops);
    if (gradientChangedCallback) gradientChangedCallback();
}

void GradientEditor::removeStop() {
    int index = gradientStrip->selectedStop();
    if (index <= 0 || index >= gradientStrip->gradientStops().size() - 1) return;
    
    auto stops = gradientStrip->gradientStops();
    stops.erase(stops.begin() + index);
    gradientStrip->setGradientStops(stops);
    if (gradientChangedCallback) gradientChangedCallback();
}

void GradientEditor::updateColor(const QColor &color) {
    currentColor = color;
    updateColorButton(color);
    
    int index = gradientStrip->selectedStop();
    if (index >= 0) {
        auto stops = gradientStrip->gradientStops();
        if (index < stops.size()) {
            stops[index].second = color;
            gradientStrip->setGradientStops(stops);
            if (gradientChangedCallback) gradientChangedCallback();
        }
    }
}

void GradientEditor::updateGradient() {
    if (previewLabel->size().isEmpty()) return;
    
    QPixmap pixmap(previewLabel->size());
    pixmap.fill(Qt::transparent);
    
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    
    auto type = static_cast<QGradient::Type>(typeCombo->currentData().toInt());
    QGradientStops stops = gradientStrip->gradientStops();
    
    if (type == QGradient::LinearGradient) {
        // Линейный градиент с углом
        qreal angle = angleSpin->value();
        qreal radians = qDegreesToRadians(angle);
        
        qreal dx = cos(radians);
        qreal dy = sin(radians);
        
        int w = pixmap.width();
        int h = pixmap.height();
        QPointF center(w / 2.0, h / 2.0);
        
        qreal length = sqrt(w*w + h*h) / 2.0;
        
        QPointF start(center.x() - dx * length, center.y() - dy * length);
        QPointF end(center.x() + dx * length, center.y() + dy * length);
        
        QLinearGradient grad(start, end);
        grad.setStops(stops);
        painter.fillRect(pixmap.rect(), grad);
    }
    else if (type == QGradient::RadialGradient) {
        // Радиальный градиент
        int w = pixmap.width();
        int h = pixmap.height();
        
        // Центр градиента
        QPointF center(w / 2.0, h / 2.0);
        
        // Радиус - половина минимального измерения
        qreal radius = qMin(w, h) / 2.0;
        
        QRadialGradient grad(center, radius);
        grad.setStops(stops);
        painter.fillRect(pixmap.rect(), grad);
    }
    else {
        // Запасной вариант
        QLinearGradient grad(0, 0, pixmap.width(), 0);
        grad.setStops(stops);
        painter.fillRect(pixmap.rect(), grad);
    }
    
    previewLabel->setPixmap(pixmap);
    if (gradientChangedCallback) gradientChangedCallback();
}

void GradientEditor::updateColorButton(const QColor &color) {
    QPixmap pm(24, 24);
    pm.fill(color);
    colorButton->setIcon(QIcon(pm));
}

QGradientStops GradientEditor::gradientStops() const {
    return gradientStrip->gradientStops();
}

QGradient::Type GradientEditor::gradientType() const {
    return static_cast<QGradient::Type>(typeCombo->currentData().toInt());
}

qreal GradientEditor::angle() const {
    return angleSpin->value();
}

bool GradientEditor::rotateWithShape() const {
    return rotateCheck->isChecked();
}