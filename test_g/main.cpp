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
#include "gradient_editor.cpp"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    auto editor = new GradientEditor();
    return a.exec();
}