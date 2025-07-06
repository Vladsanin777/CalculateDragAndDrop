#include <QVBoxLayout>
#include <QWidget>
#include "../Window/Window.cpp"

namespace MainWidget {
	class MainLayout : public QVBoxLayout {
	public:
		inline explicit MainLayout( \
			Application::CalculateDragAndDrop *app, \
			Window::Window *window \
		) noexcept;
	};
	class MainWidget : public QWidget {
	public:
		inline explicit MainWidget ( \
			Application::CalculateDragAndDrop * app, \
			Window::Window *window \
		) noexcept;
	};
}