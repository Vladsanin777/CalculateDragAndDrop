#include <vector>
#include <QPushButton>
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>


namespace CreateHistori {
	class HistoriScroll, HistoriWidget, \
		  HistoriVBox;
}
namespace LineEdit {
	class LineEdit;
}
using namespace std, CreateHistori, LineEdit;

class Window : public QWidget {
private:
	vector<HistoriScroll *> _localHistori = {};
	vector<HistoriWidget *> _resizeLocalHistori = {};
	vector<HistoriVBox *> _addLocalHistori = {};
	vector<vector<LineEdit *>> _lineEdit = {{}, {}, {}, {}, {}};
	short _inputtin[2] = {0, 0};
	vector<vector<const char*>> _result = {
		{"0"}, {"1", "2", "0"}, {"0"}, {"0"}, {"x", "0", "0"}
	};
	HistoriScroll *_globalHistori = nullptr;
	HistoriVBox *_addGlobalHistori = nullptr;
	HistoriWidget *_resizeGlobalHistori = nullptr;
	const QApplication *_app = nullptr;
public:
	explicit Window( \
			const QApplication *app = nullptr \
	) : QWidget(), _app(app) {
		setWindowTitle("CalculateDragAndDrop");
		resize(400, 800);
		setObjectName("Window");
		show();
		return;
	}
	void postInit();

	HistoriScroll* getGlobalHistori( \
	) const {
		return _globalHistori;
	}
	void setGlobalHistori( \
			HistoriScroll *newGlobalHistori \
	) {
		if (newGlobalHistori == nullptr) {
			_globalHistori = newGlobalHistori;
			setResizeGlobalHistori( \
					newGlobalHistori->getResizeHistori() \
			);
		}
		return;
	}	
	HistoriWidget* getResizeGlobalHistori( \
	) const {
		return _resizeGlobalHistori;
	}
	void setResizeGlobalHistori( \
			HistoriWidget *newResizeGlobalHistori \
	) {
		if (newResizeGlobalHistori == nullptr) {
			_resizeGlobalHistori = newResizeGlobalHistori;
			setAddGlobalHistori(newResizeGlobalHistori->getAddHistori());
		}
		return;
	}

	HistoriVBox* getAddGlobalHistori( \
	) const {
		return _addGlobalHistori;
	}
	void setAddGlobalHistori( \
			HistoriVBox *newAddGlobalHistori \
	) {
		if (newAddGlobalHistori == nullptr)
			_addGlobalHistori = newAddGlobalHistori;
		return;
	}
	HistoriScroll* getLocalHistori( \
			unsigned short *index = nullptr \
	) const {
		if (index == nullptr)
			return _localHistori.at(_inputtin[0]);
		else
			return _localHistori.at(*index);
	}
	void setLocalHistori( \
			HistoriScroll *newLocalHistori \
	) {
		if (newLocalHistori == nullptr) {
			_localHistori.push_back(newLocalHistori);
			setResizeLocalHistori(newLocalHistori->getResizeHistori());
		}
		return;
	}	
	HistoriWidget* getResizeLocalHistori( \
			unsigned short *index = nullptr \
	) const {
		if (index == nullptr)
			return _resizeLocalHistori.at(*index);
		else 
			return _resizeLocalHistori.at(_inputtin[0]);
	}
	void setResizeLocalHistori( \
			HistoriWidget *newResizeLocalHistori \
	) {
		if (newResizeLocalHistori == nullptr) {
			_resizeLocalHistori.push_back(newResizeLocalHistori);
			setAddLocalHistori(newResizeLocalHistori->getAddHistori());
		}
		return;
	}

	HistoriVBox* getAddLocalHistori( \
			unsigned short *index = nullptr \
	) const {
		if (index == nullptr)
			return _addLocalHistori.at(_inputtin[0]);
		else
			return _addLocalHistori.at(*index);
	}
	void setAddLocalHistori( \
			HistoriVBox *newAddLocalHistori \
	) {
		if (newAddLocalHistori == nullptr)
			_addLocalHistori.push_back(newAddLocalHistori);
		return;
	}
	LineEdit* getLineEdit( \
			unsigned short *tab, unsigned short *index \
	) const {
		if (tab != nullptr && index != nullptr)
			return _lineEdit.at(*tab).at(*index);
		return nullptr; 
	}
	void setInputtin(short inputtin[2]) {
		_inputtin[0] = inputtin[0];
		_inputtin[1] = inputtin[1];
	}
};




namespace GradientFont {
	class CreateGradient : public QLinearGradient {
	public:
		explicit CreateGradient( \
			Window *window, \
			QWidget *widget = nullptr, \
			vector<vector<int, QColor*>> = gradient\
				{
					{0, Qt::GlobalColor::red}, 
					{1, Qt::GlobalColor::blue}
				}
		) {
			int width = window.width();
			int height = window.height();
			if (widget != nullptr) {
				QPoint *positionWidget = \
					widget.mapToGlobal(QPoint(0, 0));
				QPoint *positionWindow = \
					window.mapToGlobal(QPoint(0, 0));
				int y = positionWidget.y() - positionWindow.y();
				int x = positionWidget.x() - positionWindow.x();
				QLinearGradient(-x, -y, width / 2, height / 2);
			} else
				QLinearGradient(0, 0, width, height);
			for (colorAt : gradient) {
				setColorAt(colorAt.at(0), colorAt.at(1));
			}
		}
	};

	class Pen : public QPen {
	public:
		explicit Pen(int width = 2)
		: QPen() {
			setColor(QColor("white"));
			setWidth(width);
		}
	};

	class Pash : public QPainterPash {
	public:
		explicit Pash( \
				QWidget *parent, \
				int textX, int textY \
		) : QPainterPash() {
			addText(textX, textY, \
					parent->font(), parent->text() \
			);
		}
	};



	class StyleBase : public QPainter {
		Q_OBJECT
	public:
		explicit Style( \
				QWidget *parent, Window *window, \
				Pash *path \
		) : QPainter() {
			setRenderHint(QPainter::RenderHint::Antialiasing);
			fillRect(parent.rect(), QColor("transparent"));
			setFont(parent.font());
			setPen(new Pen());
			setBrush(Qt::BrushStyle::NoBrush);
			drawPath(path);
			CreateGradient *gradient = GreateGradient( \
					parent, window \
			);
			setBrush(QBrush(gradient));
			setPen(Qt::PenStyle::NoPen);
			drawPath(path);
			end();
		}
	};


	class StyleLineEdit : public StyleBase {
		Q_OBJECT
	public:
		explicit StyleLineEdit( \
				QWidget *parent, Window *window \
		) {
			function<int(QWidget *, QFontMetrics *)> textX = \
			[](QWidget *parent, QFontMetrics *metrics) {
				if (metrics->horizontalAdvace(parent->text()) < parent->width())
					return (parent->width() - \
						metrics.horizontalAdvance(parent->text())) / 2;
				return 0;
			}
			function<int(QWidget *, QFontMetrics *)> textY = \
			[](QWidget *parent, QFontMetrics *metrics) {
				return (parent->height() + metrics->height()) \
					/ 2 - metrics->descent();
			}
			QFontMetrics *metrics = new QFontMetrics(parent->font());
			StyleBase( \
				parent, window, \
				new Pash( \
					parent, \
					textX(parent, metrics), \
					textY(parent, metrics) \
				) \
			);
		}
	};


	class StyleButton : public StyleBase {
		Q_OBJECT
	public:
		explicit StyleLineEdit( \
				QWidget *parent, const QWidget *window \
		) {
			function<int(QWidget *, QFontMetrics *)> textX = \
			[](QWidget *parent, QFontMetrics *metrics) {
				return parent->cursorRect()->x() - \
					metrics->horizontalAdvance( \
						parent->text()[parent->cursorPosition()] = '\0' \
					) + 5;
			}
			function<int(QWidget *, QFontMetrics *)> textY = \
			[](QWidget *parent, QFontMetrics *metrics) {
				return (parent->height() + metrics->ascent() - \
					metrics->descent()) / 2;
			}
			QFontMetrics *metrics = new QFontMetrics(parent.font());
			StyleBase( \
				parent, window, \
				new Pash( \
					parent, \
					textX(parent, metrics), \
					textY(parent, metrics) \
				) \
			);
		}
	};
}

namespace Button {
	class ButtonBase : public QPushButton {
	private:
		Window *_window
	public:
		explicit ButtonBase( \
			char label[], Window *window, short fontSize, \
			auto *callback = nullptr, \
			cssName = "keybord", QMenu *menu = nullptr \
		) : _window(window), QPushButton(label) {
			setContentsMargins(0, 0, 0, 0);
			if (callback != nullptr) 
				connect(this, &QPushButton::clicked, &QWidget, callback);
			if (menu)
				setMenu(menu);
			if (css_name)
				setObjectName(css_name);
			setFixedHeight(35);
			setMinimumWidth(64);
			QFont *font = font();
			font.setPointSize(fontSize);
			setFont(font);
		}
		void paintEvent(event) const override {
			StyleButton(this, _window);
			QPushButton::paintEvent();
			return;
		}
	};

	class ButtonDrag : public ButtonBase {
	public:
		explicit ButtonDrag( \
				char label[], Window *window, short fontSize, \
				function<void> callback = nullptr, \
				cssName = "keybord", QMenu *menu = nullptr \
		) : ButtonBase(
			label, window, fontSize, css_name, menu
		) {}
		void mousePressEvent( \
				QMouseEvent *event \
		) override const {
			_start_pos = event->pos();
			ButtonBase::mousePressEvent(event);
		}
		void mouseMoveEvent( \
				QMouseEvent *event \
		) const override {
			if ( \
					(event->pos() - _start_pos).manhattanLength() \
					> QApplication::startDragDistance() \
			) {
				QDrag *drag = QDrag(this);
				QMimeData *mime_data = QMimeData();
				mime_data->setText(this->text());
				drag->setMimeData(mime_data);
				drag->exec(Qt::DropAction::MoveAction);
			} else 
				ButtonBase::mouseMoveEvent(event);
		}
	};

	class ButtonDragAndDrop : public ButtonDrag {
	public:
		explicit ButtonDragAndDrop( \
				char label[], Window *window, short fontSize, \
				function<void> callback = nullptr, \
				cssName = "keybord", QMenu *menu = nullptr \
		) : ButtonBase( \
			label, window, fontSize, css_name, menu \
		) {}
		void dragEnterEvent( \
				QMouseEvent *event \
		) override const {
			if (event->mimeData()->hasText())
				event->acceptProposedAction();
		}
		void dropEvent( \
				QMouseEvent *event \
		) override {
			setText(event->mimeData()->text());
			event->acceptProposedAction();
		}
	};
}

namespace CreateHistori {
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
}

namespace Title {
	using namespace Button;
	class Action : public QWidgetAction {
		Q_OBJECT
	public:
		explicit Action(Menu *parent, ButtonBase *button)
			: QWidgetAction(parent) {
			setDefaultWidget(button); // Add semicolon here
		}
	};
	class Menu : public QMenu {
		Q_OBJECT
	private:
		explicit Menu( \
				vector<ButtonBase *> buttons \
		) : QMenu() {
			setAttribute( \
					Qt.WidgetAttribute.WA_TranslucentBackground \
			);
			for (button : buttons) {
				addAction(Action(this, button))
			}
		}
	};


	class TitleBar : public QHBoxLayout {
		Q_OBJECT
	private:
		QWidget    *_window                 = nullptr;
		QScrollArea *_globalHistori          = nullptr;
		QScrollArea *_localHistoriBasic      = nullptr;
		QScrollArea *_localHistoriIntegral   = nullptr;
		QScrollArea *_localHistoriDerivative = nullptr;
		QScrollArea *_localHistoriIntegrate  = nullptr;
		QScrollArea *_localHistoriReplacement = nullptr;
	public:
		explicit TitleBar( \
				const QApplication *app, Window *window \
		) : window(window), QHBoxLayout() {
			setFixedHeight(35);
			addWidget(ButtonBase("+ Add", window, 15, &app::addWindow);
			addWidget(ButtonBase("EN",    window, 15, &window::changeLanguage);
			addWidget(ButtonBase("Fon",   window, 15, &app::changeFon);
			vector<ButtonBase *> vectorButtonLocalHistori = {
				new ButtonBase("Basic",       window, 15, \
						&TitleBar::localHistoriBasicVisible \
				),
				new ButtonBase("Integral",    window, 15, \
						&TitleBar::localHistoriIntegralVisible \
				),
				new ButtonBase("Derivative",  window, 15, \
						&TitleBar::localHistoriDerivativeVisible \
				),
				new ButtonBase("Integrate",   window, 15, \
						&TitleBar::localHistoriIntegrateVisible \
				),
				new ButtonBase("Replacement", window, 15, \
						&TitleBar::localHistoriReplacementVisible \
				),
			}, vectorButtonView = {
				new ButtonBase("Global Histori", window, 15, \
						&TitleBar::globalHistoriVisible),
				new ButtonBase("Local Histori",  window, 15, \
						nullptr, "keybord", Menu(vectorButtonLocalHistori))
			};
			addWidget(ButtonBase("Basic", window, 15, \
					nullptr, "keybord", Menu(vectorButtonView)));
			_globalHistori           = window->getGlobalHistori();
			_localHistoriBasic       = window->getLocalHistori(0);
			_localHistoriIntegral    = window->getLocalHistori(1);
			_localHistoriDerivative  = window->getLocalHistori(2);
			_localHistoriIntegrate   = window->getLocalHistori(3);
			_localHistoriReplacement = window->getLocalHistori(4);
			return;
		}
		void globalHistoriVisible() const {
			_globalHistori->setVisible( \
					!_globalHistori->isVisible() \
			);
			return;
		}
		void localHistoriBasicVisible() const {
			_localHistoriBasic->setVisible( \
					!_localHistoriBasic->isVisible() \
			);
			return;
		}
		void localHistoriIntegralVisible() const {
			_localHistoriIntegral->setVisible( \
					!_localHistoriIntegral->isVisible() \
			);
			return;
		}
		void localHistoriDerivativeVisible() const {
			_localHistoriDerivative->setVisible( \
					!_localHistoriDerivative->isVisible() \
			);
			return;
		}
		void localHistoriIntegrateVisible() const {
			_localHistoriIntegrate->setVisible( \
					!_localHistoriIntegrate->isVisible()
			);
			return;
		}
		void localHistoriReplacementVisible() const {
			_localHistoriReplacement->setVisible( \
					!_localHistoriReplacement->isVisible()
			);
			return;
		}
	};
}

using namespace CreateHistori;
class MainLayout : public QVBoxLayout {
public:
	explicit MainLayout( \
			const QApplication *app, Window *window \
	) : QVBoxLayout() {
		setContentsMargins(0, 0, 0, 0);
		setSpacing(0);
		addLayout(new Title::TitleBar(app, window));
		CreateHistori::HistoriScroll *globalHistori = nullptr;
		globalHistori = new CreateHistori::HistoriScroll();
		window->setGlobalHistori(static_cast<int*>(globalHistori));
		addWidget(globalHistori);
		addWidget(MainTabWidget(window));
		addLayout(GridCalculateCommon(window));
		addWidget(TabWidgetKeybord(window));
	}
};

inline void Window::postInit() {
    setLayout(new MainLayout(_app, this));
}
