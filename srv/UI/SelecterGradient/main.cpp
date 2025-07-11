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


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    auto editor = new SelecterGradient::GradientEditor();
    return a.exec();
}