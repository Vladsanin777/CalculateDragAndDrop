#include <vector>
#include <tuple>
#include <cstdint>
#include <QPushButton>
#include <QApplication>
#include <QWidget>
#include <QPointF>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QMenu>
#include <QPen>
#include <QPainterPath>
#include <QPainter>
#include <QColor>
#include <QLineEdit>


using namespace std;

class Window : public QWidget {
private:
	vector<uintptr_t> _localHistori = {};
	vector<uintptr_t> _resizeLocalHistori = {};
	vector<uintptr_t> _addLocalHistori = {};
	vector<vector<uintptr_t>> _lineEdit = {{}, {}, {}, {}, {}};
	short _inputtin[2] = {0, 0};
	vector<vector<const char*>> _result = {
		{"0"}, {"1", "2", "0"}, {"0"}, {"0"}, {"x", "0", "0"}
	};
	uintptr_t _globalHistori;
	uintptr_t _addGlobalHistori;
	uintptr_t _resizeGlobalHistori;
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

	uintptr_t getGlobalHistori( \
	) const {
		return _globalHistori;
	}
	void setGlobalHistori( \
			uintptr_t newGlobalHistori \
	) {
		_globalHistori = newGlobalHistori;
		return;
	}	
	uintptr_t getResizeGlobalHistori( \
	) const {
		return _resizeGlobalHistori;
	}
	void setResizeGlobalHistori( \
			uintptr_t newResizeGlobalHistori \
	) {
		_resizeGlobalHistori = newResizeGlobalHistori;
		return;
	}

	uintptr_t getAddGlobalHistori( \
	) const {
		return _addGlobalHistori;
	}
	void setAddGlobalHistori( \
			uintptr_t newAddGlobalHistori \
	) {
		_addGlobalHistori = newAddGlobalHistori;
		return;
	}
	uintptr_t getLocalHistori( \
			unsigned short *index = nullptr \
	) const {
		if (index == nullptr)
			return _localHistori.at(_inputtin[0]);
		else
			return _localHistori.at(*index);
	}
	void setLocalHistori( \
			uintptr_t newLocalHistori \
	) {
		_localHistori.push_back(newLocalHistori);
		return;
	}	
	uintptr_t getResizeLocalHistori( \
			unsigned short *index = nullptr \
	) const {
		if (index == nullptr)
			return _resizeLocalHistori.at(*index);
		else 
			return _resizeLocalHistori.at(_inputtin[0]);
	}
	void setResizeLocalHistori( \
			uintptr_t newResizeLocalHistori \
	) {
		_resizeLocalHistori.push_back(newResizeLocalHistori);
		return;
	}

	uintptr_t getAddLocalHistori( \
			unsigned short *index = nullptr \
	) const {
		if (index == nullptr)
			return _addLocalHistori.at(_inputtin[0]);
		else
			return _addLocalHistori.at(*index);
	}
	void setAddLocalHistori( \
			uintptr_t newAddLocalHistori \
	) {
		_addLocalHistori.push_back(newAddLocalHistori);
		return;
	}
	uintptr_t getLineEdit( \
			unsigned short *tab, unsigned short *index \
	) const {
		if (tab != nullptr && index != nullptr)
			return _lineEdit.at(*tab).at(*index);
		return 0; 
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
			vector<tuple<int, QColor>> gradient = \
				{
					{0, Qt::GlobalColor::red}, 
					{1, Qt::GlobalColor::blue}
				}
		) {
			int width = window->width();
			int height = window->height();
			if (widget != nullptr) {
				QPoint positionWidget = \
						widget->mapToGlobal(QPoint(0, 0)), \
					positionWindow = \
						window->mapToGlobal(QPoint(0, 0));
				int y = positionWidget.y() - positionWindow.y();
				int x = positionWidget.x() - positionWindow.x();
				QLinearGradient(-x, -y, width / 2, height / 2);
			} else
				QLinearGradient(0, 0, width, height);
			short gradient_lenght = size(gradient);
			for (short index = 0; index != gradient_lenght; index++) {
				tuple<float, QColor> colorAt = gradient.at(index);
				setColorAt(get<0>(colorAt), get<1>(colorAt));
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

	class Path : public QPainterPath {
	public:
		explicit Path( \
				QWidget *parent, \
				int textX, int textY, \
				const char *text[]
		) : QPainterPath() {
			addText(textX, textY, \
					parent->font(), *text \
			);
		}
	};



	class StyleBase : public QPainter {
	public:
		StyleBase( \
				QWidget *parent, Window *window, \
				Path *path \
		) : QPainter() {
			setRenderHint(QPainter::RenderHint::Antialiasing);
			fillRect(parent->rect(), QColor("transparent"));
			setFont(parent->font());
			setPen(Pen());
			setBrush(Qt::BrushStyle::NoBrush);
			drawPath(*path);
			CreateGradient gradient = CreateGradient( \
					window, parent \
			);
			setBrush(QBrush(static_cast<QLinearGradient>(gradient)));
			setPen(Qt::PenStyle::NoPen);
			drawPath(*path);
			end();
		}
	};


	class StyleLineEdit : public StyleBase {
	public:
		explicit StyleLineEdit( \
				QLineEdit *parent, Window *window \
		) {
			function<int(QWidget *, QFontMetrics *)> textX = \
			[](QWidget *parent, QFontMetrics *metrics) {
				if (metrics->horizontalAdvance(parent->text()) < parent->width())
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
				static_cast<QWidget *>(parent), window, \
				new Path( \
					static_cast<QWidget *>(parent), \
					textX(parent, metrics), \
					textY(parent, metrics), \
					parent->text() \
				) \
			);
		}
	};


	class StyleButton : public StyleBase {
	public:
		explicit StyleButton( \
				QPushButton *parent, const QWidget *window \
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
				new Path( \
					static_cast<QWidget *>(parent), \
					textX(parent, metrics), \
					textY(parent, metrics), \
					parent->text() \
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
			const char label[], Window *window, short fontSize, \
			function<void(QPushButton)> *callback = nullptr, \
			const char *cssName = "keybord", QMenu *menu = nullptr \
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
				const char label[], Window *window, short fontSize, \
				function<void(QPushButton)> callback = nullptr, \
				const char *cssName = "keybord", QMenu *menu = nullptr \
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
				const char *label[], Window *window, short fontSize, \
				function<void(QPushButton)> callback = nullptr, \
				const char *cssName = "keybord", QMenu *menu = nullptr \
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
	class Action : public QWidgetAction {
	public:
		explicit Action(Menu *parent, Button::ButtonBase *button)
			: QWidgetAction(parent) {
			setDefaultWidget(button); // Add semicolon here
		}
	};
	class Menu : public QMenu {
	public:
		explicit Menu( \
				vector<Button::ButtonBase *> buttons \
		) : QMenu() {
			setAttribute( \
					Qt.WidgetAttribute.WA_TranslucentBackground \
			);
			short buttons_lenght = buttons.lenght;
			for (short index = 0; index != buttons_lenght; index++)
				addAction(Action(this, buttons.at(index)));
		}
	};


	class TitleBar : public QHBoxLayout {
	private:
		Window                       *_window                  = nullptr;
		CreateHistori::HistoriScroll *_globalHistori           = nullptr;
		CreateHistori::HistoriScroll *_localHistoriBasic       = nullptr;
		CreateHistori::HistoriScroll *_localHistoriIntegral    = nullptr;
		CreateHistori::HistoriScroll *_localHistoriDerivative  = nullptr;
		CreateHistori::HistoriScroll *_localHistoriIntegrate   = nullptr;
		CreateHistori::HistoriScroll *_localHistoriReplacement = nullptr;
	public:
		explicit TitleBar( \
				const QApplication *app, Window *window \
		) : window(window), QHBoxLayout() {
			setFixedHeight(35);
			addWidget(Button::ButtonBase("+ Add", window, 15, &bind(&CalculateDragAndDrop::addWindow, app);
			addWidget(Button::ButtonBase("EN",    window, 15, &bind(&Window::changeLanguage, this));
			addWidget(Button::ButtonBase("Fon",   window, 15, &bind(&Window::changeFon, this)));
			vector<Button::ButtonBase *> vectorButtonLocalHistori = {
				new Button::ButtonBase("Basic",       window, 15, \
						&bind(&TitleBar::localHistoriBasicVisible, this) \
				),
				new Button::ButtonBase("Integral",    window, 15, \
						&bind(&TitleBar::localHistoriIntegralVisible, this) \
				),
				new Button::ButtonBase("Derivative",  window, 15, \
						&bind(&TitleBar::localHistoriDerivativeVisible, this) \
				),
				new Button::ButtonBase("Integrate",   window, 15, \
						&bind(&TitleBar::localHistoriIntegrateVisible, this) \
				),
				new Button::ButtonBase("Replacement", window, 15, \
						&bind(&TitleBar::localHistoriReplacementVisible, this) \
				),
			}, vectorButtonView = {
				new Button::ButtonBase("Global Histori", window, 15, \
						&bind(&TitleBar::globalHistoriVisible, this) \
				),
				new Button::ButtonBase( \
					"Local Histori",  window, 15, nullptr, \
					"keybord", static_cast<QMenu *>( \
						new Menu(vectorButtonLocalHistori) \
					) \
				) \
			};
			addWidget( \
				new Button::ButtonBase( \
					"View", window, 15, nullptr, \
					"keybord", static_cast<QMenu *>(new Menu(vectorButtonView)) \
				) \
			);
			_globalHistori           = \
				reinterpret_cast<CreateHistori::HistoriScroll *>( \
					window->getGlobalHistori() \
				);
			_localHistoriBasic       = \
				reinterpret_cast<CreateHistori::HistoriScroll *>( \
					window->getLocalHistori(new unsigned short(0))
				);
			_localHistoriIntegral    = \
				reinterpret_cast<CreateHistori::HistoriScroll *>( \
					window->getLocalHistori(new unsigned short(1))
				);
			_localHistoriDerivative  = \
				reinterpret_cast<CreateHistori::HistoriScroll *>( \
					window->getLocalHistori(new unsigned short(2))
				);
			_localHistoriIntegrate   = \
				reinterpret_cast<CreateHistori::HistoriScroll *>( \
					window->getLocalHistori(new unsigned short(3))
				);
			_localHistoriReplacement = \
				reinterpret_cast<CreateHistori::HistoriScroll *>( \
					window->getLocalHistori(new unsigned short(4))
				);
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
		HistoriScroll *globalHistori = nullptr;
		HistoriWidget *resizeGlobalHistori = nullptr;
		window->setGlobalHistori( \
			reinterpret_cast<uintptr_t>( \
				globalHistori = new HistoriScroll() \
			) \
		);
		window->setResizeGlobalHistori( \
			reinterpret_cast<uintptr_t>(
				resizeGlobalHistori = \
					globalHistori->getResizeHistori() \
			) \
		);
		window->setAddGlobalHistori( \
			reinterpret_cast<uintptr_t>( \
				resizeGlobalHistori->getAddHistori() \
			) \
		);
		addWidget(globalHistori);
		addWidget(MainTabWidget(window));
		addLayout(GridCalculateCommon(window));
		addWidget(TabWidgetKeybord(window));
	}
};

inline void Window::postInit() {
    setLayout(new MainLayout(_app, this));
}
