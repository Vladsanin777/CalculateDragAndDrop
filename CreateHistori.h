#include <QVBoxLayout>
#include <QWidget>
#include <QScrollArea>

class Window;


class HistoriVBox : public QVBoxLayout {
	Q_OBJECT
public:
	explicit HistoriVBox() : QVBoxLayout() {
		setSpacing(0);
		setContentsMargins(0, 0, 0, 0);
		setObjectName("histori");
	}
};

class HistoriWidget : public QWidget {
	Q_OBJECT
private:
	HistoriVBox *_addHistori = nullptr;
public:
	explicit HistoriWidget() : QWidget() {
		setObjectName("histori");
		setLayout(_addHistori = new HistoriVBox());
		setContentsMargins(0, 0, 0, 0);
	}
	HistoriVBox *getAddHistori() {
		return _addHistori;
	}
};

class HistoriScroll : public QScrollArea {
	Q_OBJECT
private:
	HistoriWidget *_resizeHistori = nullptr;
public:
	explicit HistoriScroll() : QScrollArea() {
		setHorizontalScrollBarPolicy( \
				Qt::ScrollBarPolicy::ScrollBarAlwaysOff \
		);
		setObjectName("histori");
		setWidgetResizable(true);
		setMinimumHeight(100);
		setWidget(_resizeHistori = new HistoriWidget());
	}
	HistoriWidget *getResizeHistori() {
		return _resizeHistori;
	}
};
