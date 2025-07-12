#include <QApplication>

#include <QWidget>
#include <QSlider>
#include <QImage>
#include <QColor>
#include <QPainter>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <functional>
#include <iostream>
#include "GradientEditor/GradientEditor.cpp"
#include <QGradient>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Gradient gradient{};
    gradient << {0.0, Qt::green, false} << {1.0, Qt:blue, true};
    auto editor = new SelecterGradient::GradientEditor{gradient};


    return a.exec();
}