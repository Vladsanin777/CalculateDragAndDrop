#include <vector>
#include <tuple>
#include <functional>
#include <cstring>
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
#include <QString>
#include <QCursor>
#include <QFocusEvent>
#include <QRect>
#include <QDrag>
#include <QMimeData>
#include <QWidgetAction>

using namespace std;



class CalculateDragAndDrop : public QApplication {

public:
	explicit CalculateDragAndDrop( \
			int argc, char *argv[] \
	) : QApplication(argc, argv) {
		setStyleSheet(R"(
			QMenu {
                background: transparent;
                border: 1px solid white;
            }
            #histori {
                background-color: transparent;
            }
            QTabWidget::pane {
                border: none;
                background: transparent;
            }
            
            QTabBar QToolButton {
                border: none;
                background: rgba(0, 0, 0, 0.3);
                color: white; 
            }
            QTabWidget {
                background: transparent;
                border: none;
                margin: 0px;
            }
            QTabBar::tab {
                background: transparent;
                border: none;
                padding: 0px auto;
                margin: 0px;
                color: transparent;
            }
            QTabBar {
                background: transparent;
                border: none;
                margin: 0px;
            }
            QLineEdit {
                background: transparent;
                border: 1px solid white;
                text-align: center;
                color: transparent;
            }
            QScrollBar:vertical {
                background: transparent;
                border: none;
            }
            QScrollBar::handle:vertical {
                background: rgba(255, 255, 255, 0.1);
                border: none;
            }
            QScrollBar::handle:vertical:hover {
                background: rgba(255, 255, 255, 0.3);
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical{
                background: none;
                height: 0;
            }
            QTabWidget::tab-bar {
                alignment: center;
            }
        )");
		createWindow(nullptr);
	}
	void createWindow(QPushButton *button);
};


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
	CalculateDragAndDrop *_app = nullptr;
public:
	explicit Window( \
			CalculateDragAndDrop *app = nullptr \
	) : QWidget(), _app(app) {
		setWindowTitle("CalculateDragAndDrop");
		resize(400, 800);
		setObjectName("Window");
		show();
		return;
	}
	void postInit();
	
	void changeLanguage(QPushButton *button) {}

	void changeFon(QPushButton *button) {}

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


inline void CalculateDragAndDrop::createWindow( \
	QPushButton * button \
) {
	Window *win = new Window(this);
	win->postInit();
	win->show();
}

namespace GradientFont {
	class CreateGradient : public QLinearGradient {
	public:
		explicit CreateGradient( \
			const Window *window, \
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
				QString text
		) : QPainterPath() {
			addText(textX, textY, \
					parent->font(), text \
			);
		}
	};



	class StyleBase : public QPainter {
	public:
		explicit StyleBase( \
		) : QPainter() {}
		void postInit(
				QWidget *parent, const Window *window, \
				GradientFont::Path path \
		) {
			setRenderHint(QPainter::RenderHint::Antialiasing);
			fillRect(parent->rect(), QColor("transparent"));
			setFont(parent->font());
			setPen(Pen());
			setBrush(Qt::BrushStyle::NoBrush);
			drawPath(path);
			CreateGradient gradient = CreateGradient( \
					window, parent \
			);
			setBrush(QBrush(static_cast<QLinearGradient>(gradient)));
			setPen(Qt::PenStyle::NoPen);
			drawPath(path);
			end();
		}
	};


	class StyleButton : public StyleBase {
	public:
		explicit StyleButton( \
				QPushButton *parent, Window *window \
		) : StyleBase(){
			function<int(QPushButton *, QFontMetrics *)> textX = \
			[](QPushButton *parent, QFontMetrics *metrics) {
				if (metrics->horizontalAdvance(parent->text()) < parent->width())
					return (parent->width() - \
						metrics->horizontalAdvance(parent->text())) / 2;
				return 0;
			};
			function<int(QPushButton *, QFontMetrics *)> textY = \
			[](QPushButton *parent, QFontMetrics *metrics) {
				return (parent->height() + metrics->height()) \
					/ 2 - metrics->descent();
			};
			QFontMetrics *metrics = new QFontMetrics(parent->font());
			StyleBase *styleBase = new StyleBase();
			styleBase->postInit( \
				static_cast<QWidget *>(parent), window, \
				Path( \
					static_cast<QWidget *>(parent), \
					textX(parent, metrics), \
					textY(parent, metrics), \
					parent->text() \
				) \
			);
		}
	};


	class StyleLineEdit : public StyleBase {
	public:
		explicit StyleLineEdit( \
				QLineEdit *parent, const Window *window, \
				QRect rect
		) {
			function<int(QLineEdit *, QFontMetrics *, QRect)> textX = \
			[](QLineEdit *parent, QFontMetrics *metrics, QRect rect) {
				return rect.x() - \
					metrics->horizontalAdvance( \
						parent->text()[parent->cursorPosition()] = '\0' \
					) + 5;
			};
			function<int(QLineEdit *, QFontMetrics *)> textY = \
			[](QLineEdit *parent, QFontMetrics *metrics) {
				return (parent->height() + metrics->ascent() - \
					metrics->descent()) / 2;
			};
			QFontMetrics *metrics = new QFontMetrics(parent->font());
			
			StyleBase *styleBase = new StyleBase();
			styleBase->postInit(
				static_cast<QWidget *>(parent), \
				window, 
				Path(
					static_cast<QWidget*>(parent),
					textX(parent, metrics, rect),
					textY(parent, metrics),
					parent->text()
				) \
			);
		}
	};
}


namespace LineEdit {
	class  LineEdit : public QLineEdit {
	private:
		Window *_window;
		short _inputtin[2];
	public:
		explicit LineEdit ( \
				Window *window, \
				short inputtin[2], char text[] \
		) : _window(window), QLineEdit() {
			_inputtin[0] = inputtin[0];
			_inputtin[1] = inputtin[1];
			setText(QString::fromUtf8(text));
			QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			setSizePolicy(sizePolicy);
			setObjectName("keybord");
			connect( \
				this, &QLineEdit::textChanged, this, \
				new function<void(char[])>( \
					bind( \
						&LineEdit::onLineEditChanged, \
						this, placeholders::_1 \
					) \
				) \
			);
			QFont font = this->font();
			font.setPointSize(25);
			setFont(font);
			setMaximumHeight(40);
			setMinimumWidth(40);
			setContentsMargins(0, 0, 0, 0);
		}
		void focusInEvent( \
				QFocusEvent *event \
		) override {
			if (event->reason())
				_window->setInputtin(_inputtin);
			QLineEdit::focusInEvent(event);
		}
		
		void paintEvent( \
				QPaintEvent *event \
		) override {
			GradientFont::StyleLineEdit(static_cast<QLineEdit *>(this), _window, cursorRect());
			QLineEdit::paintEvent(event);
		}

	private:
		void onLineEditChanged( \
				char textLineEdit[] \
		) const {
			LogicCalculate *logicCalculate = \
				new LogicCalculate(textLineEdit, _window);
			if (strstr(textLineEdit, "_ALL") != nullptr)
				logicCalculate.button_ALL();
			else if (strstr(textLineEdit, "_O") != nullptr)
				logicCalculate.button_O();
			else if (strstr(textLineEdit, "_RES") != nullptr)
				logicCalculate.button_RES();
			else
				logicCalculate.buttonOther();
		}
	};
}

namespace Button {
	class ButtonBase : public QPushButton {
	private:
		Window *_window;
	public:
		explicit ButtonBase( \
			const char label[], Window *window, short fontSize, \
			function<void(QPushButton *)> *callback = nullptr, \
			QWidget *callback_class = nullptr, \
			const char *cssName = "keybord", QMenu *menu = nullptr \
		) : _window(window), QPushButton(label) {
			setContentsMargins(0, 0, 0, 0);
			if (callback != nullptr) 
				connect(this, &QPushButton::clicked, callback);
			if (menu)
				setMenu(menu);
			if (cssName)
				setObjectName(cssName);
			setFixedHeight(35);
			setMinimumWidth(64);
			QFont buttonFont = font();
			buttonFont.setPointSize(fontSize);
			setFont(buttonFont);
		}
		void paintEvent(QPaintEvent *event) override {
			GradientFont::StyleButton(this, _window);
			QPushButton::paintEvent(event);
			return;
		}
	};

	class ButtonDrag : public ButtonBase {
	private:
		QPoint _start_pos;
	public:
		explicit ButtonDrag( \
			const char label[], Window *window, short fontSize, \
			function<void(QPushButton *)> *callback = nullptr, \
			QWidget *callback_class = nullptr, \
			const char *cssName = "keybord", QMenu *menu = nullptr \
		) : ButtonBase(
			label, window, fontSize, callback, callback_class, cssName, menu
		) {}
		void mousePressEvent( \
				QMouseEvent *event \
		) override {
			_start_pos = event->pos();
			ButtonBase::mousePressEvent(event);
		}
		void mouseMoveEvent( \
				QMouseEvent *event \
		) override {
			if ( \
					(event->pos() - _start_pos).manhattanLength() \
					> QApplication::startDragDistance() \
			) {
				QDrag *drag = new QDrag(this);
				QMimeData *mime_data = new QMimeData();
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
			const char label[], Window *window, short fontSize, \
			function<void(QPushButton *)> *callback = nullptr, \
			QWidget *callback_class = nullptr, \
			const char *cssName = "keybord", QMenu *menu = nullptr \
		) : ButtonDrag ( \
			label, window, fontSize, callback, callback_class, cssName, menu \
		) {}
		void dragEnterEvent( \
				QDragEnterEvent *event \
		) override {
			if (event->mimeData()->hasText())
				event->acceptProposedAction();
		}
		void dropEvent( \
				QDropEvent *event \
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
		explicit Action(QMenu *parent, Button::ButtonBase *button)
			: QWidgetAction(parent) {
			setDefaultWidget(static_cast<QPushButton *>(button)); // Add semicolon here
		}
	};
	class Menu : public QMenu {
	public:
		explicit Menu( \
				vector<Button::ButtonBase *> buttons \
		) : QMenu() {
			setAttribute( \
					Qt::WidgetAttribute::WA_TranslucentBackground \
			);
			short buttons_lenght = buttons.size();
			for (short index = 0; index != buttons_lenght; index++)
				addAction( \
					static_cast<QWidgetAction *>( \
						new Action( \
							static_cast<QMenu *>(this), \
							buttons.at(index) \
						) \
					) \
				);
		}
	};


	class TitleLayout : public QHBoxLayout {
	private:
		const Window                 *_window                  = nullptr;
		CreateHistori::HistoriScroll *_globalHistori           = nullptr;
		CreateHistori::HistoriScroll *_localHistoriBasic       = nullptr;
		CreateHistori::HistoriScroll *_localHistoriIntegral    = nullptr;
		CreateHistori::HistoriScroll *_localHistoriDerivative  = nullptr;
		CreateHistori::HistoriScroll *_localHistoriIntegrate   = nullptr;
		CreateHistori::HistoriScroll *_localHistoriReplacement = nullptr;
	public:
		explicit TitleLayout( \
				CalculateDragAndDrop *app, Window *window \
		) : _window(window), QHBoxLayout() {
			addWidget( \
				static_cast<QWidget *>( \
					new Button::ButtonBase( \
						"+ Add", window, 15, \
						new function<void(QPushButton *)>( \
							bind( \
								&CalculateDragAndDrop::createWindow, app, placeholders::_1 \
							) \
						) \
					) \
				) \
			);
			addWidget( \
				static_cast<QWidget *>( \
					new Button::ButtonBase( \
						"EN",    window, 15, \
						new function<void(QPushButton *)>( \
							bind( \
								&Window::changeLanguage, window, placeholders::_1 \
							) \
						), static_cast<QWidget *>(window)\
					) \
				) \
			);
			addWidget( \
				static_cast<QWidget *>( \
					new Button::ButtonBase( \
						"Fon",   window, 15, \
						new function<void(QPushButton *)>( \
							bind( \
								&Window::changeFon, window, placeholders::_1 \
							) \
						) \
					) \
				) \
			);
			vector<Button::ButtonBase *> vectorButtonLocalHistori = {
				new Button::ButtonBase( \
					"Basic",       window, 15, \
					new function<void(QPushButton *)>( \
						bind( \
							&TitleLayout::localHistoriBasicVisible, \
							this, placeholders::_1 \
						) \
					) \
				),
				new Button::ButtonBase( \
					"Integral",    window, 15, \
					new function<void(QPushButton *)>( \
						bind( \
							&TitleLayout::localHistoriIntegralVisible \
							, this, placeholders::_1 \
						) \
					) \
				),
				new Button::ButtonBase( \
					"Derivative",  window, 15, \
					new function<void(QPushButton *)>( \
						bind( \
							&TitleLayout::localHistoriDerivativeVisible, \
							this, placeholders::_1 \
						) \
					) \
				),
				new Button::ButtonBase( \
					"Integrate",   window, 15, \
					new function<void(QPushButton *)>( \
						bind( \
							&TitleLayout::localHistoriIntegrateVisible, \
							this, placeholders::_1 \
						) \
					) \
				),
				new Button::ButtonBase( \
					"Replacement", window, 15, \
					new function<void(QPushButton *)>( \
						bind( \
							&TitleLayout::localHistoriReplacementVisible, \
							this, placeholders::_1 \
						) \
					) \
				),
			}, vectorButtonView = {
				new Button::ButtonBase( \
					"Global Histori", window, 15, \
					new function<void(QPushButton *)>( \
						bind(
							&TitleLayout::globalHistoriVisible, \
							this, placeholders::_1 \
						) \
					) \
				),
				new Button::ButtonBase( \
					"Local Histori",  window, 15, nullptr, \
					"keybord", static_cast<QMenu *>( \
						new Menu(vectorButtonLocalHistori) \
					) \
				) \
			};
			addWidget( \
				static_cast<QWidget *>( \
					new Button::ButtonBase( \
						"View", window, 15, nullptr, \
						"keybord", static_cast<QMenu *>(new Menu(vectorButtonView)) \
					) \
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
		void globalHistoriVisible(QPushButton *button) const {
			_globalHistori->setVisible( \
					!_globalHistori->isVisible() \
			);
			return;
		}
		void localHistoriBasicVisible(QPushButton *button) const {
			_localHistoriBasic->setVisible( \
					!_localHistoriBasic->isVisible() \
			);
			return;
		}
		void localHistoriIntegralVisible(QPushButton *button) const {
			_localHistoriIntegral->setVisible( \
					!_localHistoriIntegral->isVisible() \
			);
			return;
		}
		void localHistoriDerivativeVisible(QPushButton *button) const {
			_localHistoriDerivative->setVisible( \
					!_localHistoriDerivative->isVisible() \
			);
			return;
		}
		void localHistoriIntegrateVisible(QPushButton *button) const {
			_localHistoriIntegrate->setVisible( \
					!_localHistoriIntegrate->isVisible()
			);
			return;
		}
		void localHistoriReplacementVisible(QPushButton *button) const {
			_localHistoriReplacement->setVisible( \
					!_localHistoriReplacement->isVisible()
			);
			return;
		}
	};
	class TitleBar : public QWidget {
	public:
		explicit TitleBar( \
			CalculateDragAndDrop *app, Window *window \
		) : QWidget() {
			setFixedHeight(35);
			setLayout( \
				static_cast<QHBoxLayout *>(new TitleLayout(app, window)) \
			);
		}
	};
}

using namespace CreateHistori;
class MainLayout : public QVBoxLayout {
public:
	explicit MainLayout( \
		CalculateDragAndDrop *app, Window *window \
	) : QVBoxLayout() {
		setContentsMargins(0, 0, 0, 0);
		setSpacing(0);
		addWidget(new Title::TitleBar(app, window));
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
