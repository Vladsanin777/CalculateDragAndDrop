#include "UI/Application/Application.cpp"

		
int main(int argc, char *argv[]) {
	qputenv("QT_MESSAGE_PATTERN", "%{file}:%{line} - %{message}");
	qSetMessagePattern("%{function}(): %{message}");
	Application::CalculateDragAndDrop * app \
		{new Application::CalculateDragAndDrop(argc, argv)};
	app->exec();
	delete app;
	return 0;
}
