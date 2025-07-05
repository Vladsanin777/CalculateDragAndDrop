#include <QVBoxLayout>
#include <QWidget>

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