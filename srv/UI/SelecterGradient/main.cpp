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
    Theme::Gradient gradient{};
    gradient << Theme::GradientPoint{0.0, QColor{130, 15, 8}} \
    << Theme::GradientPoint{0.3, QColor{0, 255, 0}} \
    << Theme::GradientPoint{0.5, Qt::red} \
    << Theme::GradientPoint{1.0, Qt::blue};
    auto editor = new SelecterGradient::GradientEditor{gradient};


    return a.exec();
}