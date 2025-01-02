// g++ -o main main.cpp -I/usr/include/qt6 -I/usr/include/qt6/QtCore -I/usr/include/qt6/QtGui -I/usr/include/qt6/QtWidgets -lQt6Core -lQt6Gui -lQt6Widgets

#include <QApplication>
#include "Window.h"


		
int main(int argc, char *argv[]) {
	CalculateDragAndDrop *app = new CalculateDragAndDrop(argc, argv);
	app->exec();
	delete app;
	return 0;
}
