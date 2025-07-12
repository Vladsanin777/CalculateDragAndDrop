#pragma once
#include "GradientEditor.hpp"


namespace SelecterGradient {
    GradientEditor::GradientEditor(Gradient &gradient, QWidget *parent) 
        : QWidget(parent), _gradient{gradient}, \
        _currentColor(gradient[0].getColor()) {
        setupUI();
        
        // Инициализация градиента
        //_gradientStrip = new GradientStrip{_gradient};
        
        /*
        gradientStrip->setStopSelectedCallback([this](int index) {
            if (index >= 0 && index < gradientStrip->gradientStops().size()) {
                currentColor = gradientStrip->gradientStops().at(index).second;
                updateColorButton(currentColor);
                removeButton->setEnabled(index != 0 && 
                                        index != gradientStrip->gradientStops().size() - 1);
            }
        });
        */
        
        /*
        gradientStrip->setStopsChangedCallback([this] {
            updateGradient();
        });
        */
        show();
    }

    void GradientEditor::setGradientChangedCallback(GradientChangedCallback callback) {
        _gradientChangedCallback = callback;
    }

    void GradientEditor::setupUI() {
        QGridLayout *layout = new QGridLayout(this);
        
        // Тип градиента
        layout->addWidget(new QLabel(QObject::tr("Type:")), 0, 0);
        _typeCombo = new QComboBox(this);
        _typeCombo->addItem(QObject::tr("Linear"), QGradient::LinearGradient);
        _typeCombo->addItem(QObject::tr("Radial"), QGradient::RadialGradient);
        _typeCombo->addItem(QObject::tr("Conical"), QGradient::ConicalGradient);
        _typeCombo->addItem(QObject::tr("No"), QGradient::NoGradient);
        layout->addWidget(_typeCombo, 1, 0);
        
        // Угол (для линейного градиента)
        _angleLabel = new QLabel(QObject::tr("Angle:"), this);
        layout->addWidget(_angleLabel, 0, 1);

        _angleSpin = new QDoubleSpinBox(this);
        _angleSpin->setRange(0, 360);
        _angleSpin->setValue(90);
        _angleSpin->setSuffix("°");
        layout->addWidget(_angleSpin, 1, 1);
        
        // Центр (для радиального градиента)
        _centerLabel = new QLabel(QObject::tr("Center:"), this);
        layout->addWidget(_centerLabel, 0, 1); // Та же позиция, что и angleLabel

        _centerCombo = new QComboBox(this);
        _centerCombo->addItem(QObject::tr("center"), "center");
        _centerCombo->addItem(QObject::tr("top-left"), "top-left");
        _centerCombo->addItem(QObject::tr("top-right"), "top-right");
        _centerCombo->addItem(QObject::tr("bottom-left"), "bottom-left");
        _centerCombo->addItem(QObject::tr("bottom-right"), "bottom-right");
        layout->addWidget(_centerCombo, 1, 1); // Та же позиция, что и angleSpin
        
        // Предпросмотр
        _previewLabel = new QLabel(this);
        _previewLabel->setMinimumSize(150, 150); // Увеличим для лучшего отображения
        layout->addWidget(new QLabel(QObject::tr("Предварительный просмотр:")), 0, 4);
        layout->addWidget(_previewLabel, 0, 3);
        
        // Кнопки управления
        _addButton = new QPushButton(QObject::tr("Добавить"), this);
        _removeButton = new QPushButton(QObject::tr("Удалить"), this);
        _removeButton->setEnabled(false);
        
        QHBoxLayout *buttonsLayout = new QHBoxLayout;
        buttonsLayout->addWidget(_addButton);
        buttonsLayout->addWidget(_removeButton);
        layout->addLayout(buttonsLayout, 2, 0, 1, 2);
        
        // Кнопка выбора цвета
        _colorButton = new QPushButton(this);
        _colorButton->setFixedSize(30, 30);
        updateColorButton(Qt::black);
        layout->addWidget(new QLabel(QObject::tr("Цвет:")), 2, 2);
        layout->addWidget(_colorButton, 2, 3);
        
        // Полоса градиента
        _gradientStrip = new GradientStrip(_gradient, this);
        layout->addWidget(_gradientStrip, 1, 0, 1, 6);
        
        
        // Соединения
        connect(_addButton, &QPushButton::clicked, this, [this] { addPoint(); });
        connect(_removeButton, &QPushButton::clicked, this, [this] { removePoint(); });
        connect(_typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, [this] { 
                    bool isLinear = (_typeCombo->currentData().toInt() == QGradient::LinearGradient);
                    
                    // Переключаем видимость настроек
                    _angleLabel->setVisible(isLinear);
                    _angleSpin->setVisible(isLinear);
                    _centerLabel->setVisible(!isLinear);
                    _centerCombo->setVisible(!isLinear);
                    
                    updateGradient(); 
                });
        connect(_angleSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
                this, [this] { updateGradient(); });
        connect(_centerCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
                this, [this] { updateGradient(); });
        
        connect(_colorButton, &QPushButton::clicked, this, [this] {
            auto menu = new SelecterColor::ColorPicker(_currentColor, this);
            //menu->setColorSelectedCallback([this](const QColor &color) { \
                updateColor(color); \
            });
            //menu->popup(colorButton->mapToGlobal(QPoint(0, colorButton->height())));
        });
        
        // Инициализация видимости
        bool isLinear = (_typeCombo->currentData().toInt() == QGradient::LinearGradient);
        _angleLabel->setVisible(isLinear);
        _angleSpin->setVisible(isLinear);
        _centerLabel->setVisible(!isLinear);
        _centerCombo->setVisible(!isLinear);
    }


    void GradientEditor::addPoint() {
        size_t index {_gradientStrip->getSelectedIndex()};
        if (index < 0) return;
        
        if (index >= _gradient.size() - 1) return;
        
        GradientPoint point0 {_gradient[index]};
        GradientPoint point1 {_gradient[index+1]};

        qreal position = (point0.getPosition() + point1.getPosition()) / 2;
        QColor color0 {point0.getColor()};
        QColor color1 {point1.getColor()};
        QColor color {QColor::fromRgbF(
            (color0.redF() + color1.redF()) / 2,
            (color0.greenF() + color1.greenF()) / 2,
            (color0.blueF() + color1.blueF()) / 2
        )};
        
        _gradient.insert(_gradient.begin() + index + 1, {position, color});
        // if (_gradientChangedCallback) _gradientChangedCallback();
    }

    void GradientEditor::removePoint() {
        int index = _gradientStrip->getSelectedIndex();
        _gradientStrip->removePoint(index);
    }

    void GradientEditor::updateColor(const QColor &color) {
        _currentColor = color;
        updateColorButton(color);
        
        int index = _gradientStrip->selectedPoint();
        if (index >= 0) {
            auto stops = _gradientStrip->gradientStops();
            if (index < _gradient.size()) {
                gradient[index].setcolor;
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
            // Радиальный градиент с выбором центра
            int w = pixmap.width();
            int h = pixmap.height();
            
            // Определяем центр в зависимости от выбора
            QPointF center;
            QString centerType = centerCombo->currentData().toString();
            
            if (centerType == "top-left") {
                center = QPointF(0, 0);
            } else if (centerType == "top-right") {
                center = QPointF(w, 0);
            } else if (centerType == "bottom-left") {
                center = QPointF(0, h);
            } else if (centerType == "bottom-right") {
                center = QPointF(w, h);
            } else { // "center"
                center = QPointF(w / 2.0, h / 2.0);
            }
            
            // Рассчитываем максимальный радиус (до самого дальнего угла)
            qreal maxRadius = 0;
            QPointF corners[] = {
                QPointF(0, 0),
                QPointF(w, 0),
                QPointF(0, h),
                QPointF(w, h)
            };
            
            for (const auto& corner : corners) {
                qreal distance = QLineF(center, corner).length();
                if (distance > maxRadius) maxRadius = distance;
            }
            
            // Создаем радиальный градиент
            QRadialGradient grad(center, maxRadius);
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
}