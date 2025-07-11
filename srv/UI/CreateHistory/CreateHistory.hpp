#include <QVBoxLayout>
#include <QWidget>
#include <QScrollArea>

#include "../Window/Window.cpp"

namespace Window {
	class Window;
}

class QVBoxLayout;
class QWidget;
class QScrollArea;

namespace CreateHistory {
	class HistoryVBox;
	class HistoryWidget;
	class HistoryScroll;
}

namespace CreateHistory {
	class HistoryVBox : public QVBoxLayout {
	public:
		explicit HistoryVBox( \
			const char * label, Window::Window *window \
		);
	};
	class HistoryWidget : public QWidget {
	private:
		HistoryVBox *_addHistory = nullptr;
	public:
		explicit HistoryWidget( \
			const char * label, Window::Window * window \
		);
		HistoryVBox *getAddHistory(void);
	};
	class HistoryScroll : public QScrollArea {
	private:
		HistoryWidget *_resizeHistory = nullptr;
	public:
		explicit HistoryScroll( \
			const char * label, Window::Window * window \
		);
		HistoryWidget *getResizeHistory(void);
	};
}