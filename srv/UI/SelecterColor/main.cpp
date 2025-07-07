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
#include "colorpicker.cpp"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    
    ColorPicker picker;
    picker.setWindowTitle("Color Picker");
    picker.resize(500, 300);
    picker.show();
    
    return a.exec();
}