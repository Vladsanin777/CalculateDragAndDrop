// g++ -g -fPIC -o main main.cpp -I/usr/include/qt6 -I/usr/include/qt6/QtCore -I/usr/include/qt6/QtGui -I/usr/include/qt6/QtWidgets -lQt6Core -lQt6Gui -lQt6Widgets -lgmp -lmpfr


#include "UI/UI.hpp"


		
int main(int argc, char *argv[]) {
	qputenv("QT_MESSAGE_PATTERN", "%{file}:%{line} - %{message}");
	qSetMessagePattern("%{function}(): %{message}");
	Application::CalculateDragAndDrop *app \
		{new Application::CalculateDragAndDrop(argc, argv)};
	app->exec();
	delete app;
	return 0;
}
