#include <QVBoxLayout>
#include <QWidget>

#include "../Window/Window.cpp"
#include "../Application/Application.cpp"

namespace Window {
	class Window;
}
namespace Application {
	class CalculateDragAndDrop;
}

class QVBoxLayout;
class QWidget;

namespace MainWidget {
	class MainLayout;
	class MainWidget;
}

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