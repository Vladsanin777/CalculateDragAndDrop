#include <QVBoxLayout>
#include <QWidget>
#include <QScrollArea>

namespace CreateHistori {
	class HistoriVBox : public QVBoxLayout {
	public:
		explicit HistoriVBox( \
			const char * label, Window::Window *window \
		);
	};
	class HistoriWidget : public QWidget {
	private:
		HistoriVBox *_addHistori = nullptr;
	public:
		explicit HistoriWidget( \
			const char * label, Window::Window * window \
		);
		HistoriVBox *getAddHistori(void);
	};
	class HistoriScroll : public QScrollArea {
	private:
		HistoriWidget *_resizeHistori = nullptr;
	public:
		explicit HistoriScroll( \
			const char * label, Window::Window * window \
		);
		HistoriWidget *getResizeHistori(void);
	};
}