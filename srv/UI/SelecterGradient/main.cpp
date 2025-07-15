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
#include "GradientStrip/GradientStrip.hpp"
#include <QGradient>


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    SelecterGradient::Gradient gradient{};
    gradient << SelecterGradient::GradientPoint{0.0, Qt::green} \
    << SelecterGradient::GradientPoint{1.0, Qt::blue};
    auto editor = new SelecterGradient::GradientEditor{gradient};


    return a.exec();
}