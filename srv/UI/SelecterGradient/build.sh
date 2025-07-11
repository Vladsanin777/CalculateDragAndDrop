#!/bin/bash
g++ -g -fPIC -o main main.cpp -I/usr/include/qt6 -I/usr/include/qt6/QtCore -I/usr/include/qt6/QtGui -I/usr/include/qt6/QtWidgets -lQt6Core -lQt6Gui -lQt6Widgets -lgmp -lmpfr