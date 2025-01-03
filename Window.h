#include <vector>
#include <tuple>
#include <functional>
#include <cstring>
#include <cstdint>
#include <iostream>
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
#include <QPaintEvent>
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
			QPushButton {
				background-color: transparent;
				color: transparent;
				border: none;
			}
        )");
		createWindow(nullptr);
	}
	void createWindow(QPushButton *button);
};

class CreateGradient : public QLinearGradient {
public:
	explicit CreateGradient( \
		QWidget *window, \
		QWidget *widget = nullptr, \
		vector<tuple<float, QColor>> gradient = \
			{
				{0.0f, Qt::GlobalColor::red}, 
				{1.0f, Qt::GlobalColor::blue}
			}
	) : QLinearGradient() {
		 if (window == nullptr) {
            // Выводите ошибку или возвращайте пустой объект
            qWarning("Window is nullptr!");
            return;
        }

		int width = window->width();
		int height = window->height();
		if (widget != nullptr) {
			QPoint positionWidget = \
					widget->mapToGlobal(QPoint(0, 0)), \
				positionWindow = \
					window->mapToGlobal(QPoint(0, 0));
			int y = positionWidget.y() - positionWindow.y();
			int x = positionWidget.x() - positionWindow.x();
			setStart(-x, -y);
			setFinalStop(width / 2, height / 2);
		} else {
			setStart(0, 0);
			setFinalStop(width, height);
		}
		short gradient_lenght = gradient.size();
		for (short index = 0; index != gradient_lenght; index++) {
			tuple<float, QColor> colorAt = gradient.at(index);
			// cout << get<0>(colorAt) << endl;
			setColorAt(get<0>(colorAt), get<1>(colorAt));
		}
		// cout<<endl<<endl;
	}
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

	void paintEvent(QPaintEvent *event) override {
        // Создаём QPainter для рисования
        QPainter painter = QPainter(this);
        painter.setRenderHint(QPainter::RenderHint::Antialiasing);

        // Применяем градиент как кисть и заполняем окно
        painter.fillRect( \
			rect(), \
            QBrush(CreateGradient( \
                static_cast<QWidget *>(this), nullptr, \
                vector<tuple<float, QColor>>{
					{0.0f, QColor(100, 0, 0)},
					{0.5f, QColor(0, 0, 0)},
					{1.0f, QColor(0, 0, 100)}
				}
            )) \
        );
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
		if (index == nullptr) {
			if (_inputtin[0] >= _localHistori.size()) {
				std::cerr << "Ошибка: индекс вне диапазона вектора _inputtin или _localHistori!" << std::endl;
				return 0; // или другое значение по умолчанию
			}
			return _localHistori.at(_inputtin[0]);
		} else {
			if (*index >= _localHistori.size()) {
				std::cerr << "Ошибка: индекс вне диапазона вектора _localHistori!" << std::endl;
				return 0; // или другое значение по умолчанию
			}
			return _localHistori.at(*index);
		}
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
		if (index == nullptr) {
			if (_inputtin[0] >= _resizeLocalHistori.size()) {
				std::cerr << "Ошибка: индекс вне диапазона вектора _inputtin или _localHistori!" << std::endl;
				return 0; // или другое значение по умолчанию
			}
			return _resizeLocalHistori.at(_inputtin[0]);
		} else {
			if (*index >= _resizeLocalHistori.size()) {
				std::cerr << "Ошибка: индекс вне диапазона вектора _localHistori!" << std::endl;
				return 0; // или другое значение по умолчанию
			}
			return _resizeLocalHistori.at(*index);
		}
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
		if (index == nullptr) {
			if (_inputtin[0] >= _addLocalHistori.size()) {
				std::cerr << "Ошибка: индекс вне диапазона вектора _inputtin или _localHistori!" << std::endl;
				return 0; // или другое значение по умолчанию
			}
			return _addLocalHistori.at(_inputtin[0]);
		} else {
			if (*index >= _addLocalHistori.size()) {
				std::cerr << "Ошибка: индекс вне диапазона вектора _localHistori!" << std::endl;
				return 0; // или другое значение по умолчанию
			}
			return _addLocalHistori.at(*index);
		}
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
				QWidget *parent, Window *window, \
				GradientFont::Path path \
		) {
			if (!begin(parent)) {  // Активируем QPainter для родительского виджета
				qWarning() << "Failed to begin QPainter";
				return;
			}
			setRenderHint(QPainter::RenderHint::Antialiasing);
			fillRect(parent->rect(), QColor("transparent"));
			setFont(parent->font());
			setPen(Pen());
			setBrush(Qt::BrushStyle::NoBrush);
			drawPath(path);
			CreateGradient gradient = CreateGradient( \
				static_cast<QWidget *>(window), parent \
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
				QLineEdit *parent, Window *window, \
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
				&LineEdit::onLineEditChanged \
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
				const QString& text \
		) const {

			// Преобразуем QString в QByteArray
			QByteArray byteArray = text.toUtf8();

			// Копируем данные QByteArray в char[]
			char textCh[byteArray.size() + 1]; // +1 для нулевого завершающего символа
			strcpy(textCh, byteArray.constData());
			/*
			LogicCalculate *logicCalculate = \
				new LogicCalculate(textCh, _window);
			if (strstr(textCh, "_ALL") != nullptr)
				logicCalculate.button_ALL();
			else if (strstr(textCh, "_O") != nullptr)
				logicCalculate.button_O();
			else if (strstr(textCh, "_RES") != nullptr)
				logicCalculate.button_RES();
			else
				logicCalculate.buttonOther();
			*/
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
			const char *cssName = "keybord", QMenu *menu = nullptr \
		) : _window(window), QPushButton(label) {
			setContentsMargins(0, 0, 0, 0);
			if (callback != nullptr) 
				connect(this, &QPushButton::clicked, [this, callback](bool) {
					if (callback) {
						(*callback)(this);
					}
				});
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
			const char *cssName = "keybord", QMenu *menu = nullptr \
		) : ButtonBase(
			label, window, fontSize, callback, cssName, menu
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
			const char *cssName = "keybord", QMenu *menu = nullptr \
		) : ButtonDrag ( \
			label, window, fontSize, callback, cssName, menu \
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
	public:
		explicit HistoriVBox() : QVBoxLayout() {
			setSpacing(0);
			setContentsMargins(0, 0, 0, 0);
			setObjectName("histori");
		}
	};

	class HistoriWidget : public QWidget {
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
			// Проверка на пустой вектор
			if (buttons.empty()) {
				std::cerr << "Ошибка: передан пустой вектор кнопок!" << std::endl;
				return;
			}
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
			setContentsMargins(0, 0, 0, 0);
        	setSpacing(0);
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
						) \
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

namespace Grid {
	class BuildingGridKeybord():
		def __init__(self, list_button: list[list[str]], grid: QGridLayout, window, row: int = 0, *, button = ButtonDrag):
			for row_labels_for_button in list_button:
				column: int = 0
				for one_button in row_labels_for_button:
					grid.addWidget(button(one_button, callback = LogicCalculate.inputing_line_edit, window = window), row, column, 1, 1)
					column += 1
				row += 1

	class GridCalculateKeybord(QGridLayout):
		def __init__(self, list_button: list[list[str]], window):
			super().__init__()

			self.setContentsMargins(0, 0, 0, 0)
			self.setSpacing(0)
			BuildingGridKeybord(list_button, self, window)


	class GridCalculateCommon(QGridLayout):
		def __init__(self, window):
			super().__init__()
			self.setSpacing(0)
			self.setContentsMargins(0, 0, 0, 0)

			self.window = window

			self.button("_ALL", 0, 0)

			self.button("_DO", 0, 1)
			self.button("_RES", 0, 2)
			self.button("_POS", 0, 3)
			self.button("_O", 0, 4)
			BuildingGridKeybord([
				["()", "(", ")", "mod", "_PI"], 
				["7", "8", "9", ":", "sqrt"], 
				["4", "5", "6", "*", "^"], 
				["1", "2", "3", "-", "!"], 
				["0", ".", "%", "+", "_E"],
				["", "", "", "", ""]
			], self, window, 1, button = ButtonDragAndDrop)
			window.set_for_result = ButtonDrag(window.result[0][0], window = window)
			self.addWidget(window.set_for_result, 7, 0, 1, 5) 

		def button(self, label: str, row: int, column: int, *, button = ButtonDrag) -> None:
			self.addWidget(button(label, css_name = "keybord", callback = LogicCalculate.inputing_line_edit, window = self.window), row, column, 1, 1)

	class GridBaseCalc(QGridLayout):
		def __init__(self, window):
			super().__init__()
			self.setSpacing(0)
			self.setContentsMargins(0, 0, 0, 0)
			local_histori = HistoriScroll()
			window.local_histori = local_histori
			self.addWidget(local_histori, 0, 0, 1, 6)

	class GridBasicCalc(GridBaseCalc):
		def __init__(self, window):
			super().__init__(window)
			line_edit = LineEdit(window, (0, 0))
			window.line_edit = 0, line_edit
			self.addWidget(line_edit)


	class GridIntegralCalc(GridBaseCalc):
		def __init__(self, window):
			super().__init__(window)

			self.addWidget(ButtonBase("a = ", css_name = "calculate", width = 64, window = window), 1, 0, 1, 1)
			a_line_edit = LineEdit(window, (1, 0), text = "1")
			window.line_edit = 1, a_line_edit
			self.addWidget(a_line_edit, 1, 1, 1, 2)
			self.addWidget(ButtonBase("b = ", css_name = "calculate", width = 64, window = window), 1, 3, 1, 1)
			b_line_edit = LineEdit(window, (1, 1), text = "2")
			window.line_edit = 1, b_line_edit
			self.addWidget(b_line_edit, 1, 4, 1, 2)
			main_line_edit = LineEdit(window, (1, 2))
			window.line_edit = 1, main_line_edit
			self.addWidget(main_line_edit, 2, 0, 1, 6)

	class GridDerivativeOrIntegrateCalc(GridBaseCalc):
		def __init__(self: Self, window, number_tab: int):
			super().__init__(window)
			line_edit = LineEdit(window, (number_tab, 0))
			window.line_edit = number_tab, line_edit
			self.addWidget(line_edit)
	class GridReplacementCalc(GridBaseCalc):
		def __init__(self: Self, window):
			super().__init__(window)
			self.addWidget(ButtonBase("with =", css_name = "calculate", width = 100, window = window), 1, 0, 1, 1)
			with_line_edit = LineEdit(window, (4, 0), text = "x")
			window.line_edit = 4, with_line_edit
			self.addWidget(with_line_edit, 1, 1, 1, 2)
			self.addWidget(ButtonBase("on =", css_name = "calculate", width = 100, window = window), 1, 3, 1, 1)
			on_line_edit = LineEdit(window, (4, 1), text = "0")
			window.line_edit = 4, on_line_edit
			self.addWidget(on_line_edit, 1, 4, 1, 2)
			main_line_edit = LineEdit(window, (4, 2))
			window.line_edit = 4, main_line_edit
			self.addWidget(main_line_edit, 2, 0, 1, 6) 


}


using namespace Grid;

namespace TabWindow {
	class CustomTabBar : public QTabBar {
	private:
		CreateGradient *_gradient = nullptr;
		QTabWidget *_tabWidget = nullptr;
		Window *_window = nullptr;
	public:
		explicit CustomTabBar( \
			QTabWidget *tabWidget, Window *window \
		) : QTabBar() {
			_tabWidget = tabWidget;
			_window = window;
			QFont fontTabBar = font();
			fontTabBar.setPointSize(20);
			setFont(fontTabBar);
		}

		void create_style() {
			// Задаем градиенты
			_gradient = CreateGradient(_tabWidget, _window)
		}
		void paintEvent(QPaintEvent *event) override {
			painter = QPainter(this);
			painter.setRenderHint(QPainter::RenderHint::Antialiasing);
			short countTabs = static_cast<short>(count());
			for (int index; index != count(); index++) {
				QRect rect = tabRect(index);
				bool is_selected = currentIndex() == index;

				// Установка шрифта
				QFont fontTabBar = font();
				painter.setFont(fontTabBar);

				// Получение текста вкладки
				QString text = self.tabText(index);
				QFontMetrics metrics = QFontMetrics(font);
				short textWidth = metrics.horizontalAdvance(text);
				short textHeight = metrics.height();

				// Центрирование текста внутри вкладки
				short x = rect.x() + (rect.width() - text_width) / 2;
				short y = rect.y() + (rect.height() + text_height) \
					/ 2 - metrics.descent();

				// Создаём путь текста
				QPainterPash path = QPainterPath();
				path.addText(x, y, font, text);

				// Рисуем текст разным цветом для активной вкладки
				QPen pen = QPen();
				pen.setColor(QColor("white"));  // Цвет текста неактивной вкладки
				if (isSelected)
					pen.setWidth(4);
				else
					pen.setWidth(2);
				painter.setPen(pen);
				painter.setBrush(Qt::BrushStyle::NoBrush);
				painter.drawPath(path);

				// Установка градиента для вкладки
				painter.setBrush(QBrush(_gradient));
				painter.setPen(Qt::PenStyle::NoPen);
				painter.drawPath(path);
			}
			painter.end();
		}
	};



	class TabQWidget : public QWidget {
	public:
		explicit TabQWidget( \
			Grid::GridCalculateKeybord tab \
		) : QWidget() {
			setLayout(tab);
		}
	};


	class TabWidgetKeyboard : public QTabWidget {
	private:
		Window *_window = nullptr;
		CustomTabBar _tabBar;
	public:
		explicit TabWidgetKeyboard( \
			Window *window \
		) : QTabWidget() {
			_window = window;
			setFixedHeight(110);
			_tabBar = CustomTabBar(this, window);
			setTabBar(tab_bar);  // Устанавливаем кастомный TabBar
			addTab(TabQWidget( \
				GridCalculateKeybord( \
					vector<vector<char[]>>{
						{"1", "2", "3", "4", "5"},
						{"6", "7", "8", "9", "0"}
					}, window \
				) \
			), "digits 10");
			addTab(TabQWidget( \
				GridCalculateKeybord( \
					vector<vector<char[]>>{
						{"A", "B", "C"},
						{"D", "E", "F"}
					}, window \
				) \
			), "digits 16");
			addTab(TabQWidget( \
				GridCalculateKeybord( \
					vector<vector<char[]>{
						{"+", "-", ":", "*", "^"},
						{"!", "sqrt", "ln", "log", "lg"}
					}, window \
				) \
			), "operators");
			addTab(TabQWidget( \
				GridCalculateKeybord( \
					vector<vector<char[]>>{
						{"_E", "_PI"}
					}, window \
				) \
			), "consts");
			addTab(TabQWidget( \
				GridCalculateKeybord( \
					vector<vector<char[]>>{
						{"sin(", "cos(", "tan("},
						{"sec(", "csc(", "cot("}
					}, window \
				) \
			), "trigonometric functions");
			addTab(TabQWidget( \
				GridCalculateKeybord( \
					vector<vector<char[]>>{
						{"sgn(", "abs(", "mod"}
					}, window \
				) \
			), "other functions");
			addTab(TabQWidget( \
				GridCalculateKeybord( \
					vector<vector<char[]>>{
						{"0x", "0b", "0t"}
					}, window \
				) \
			), "number system");
			addTab(TabQWidget( \
				GridCalculateKeybord( \
					vector<vector<char[]>>{
						{"%", "mod", ".", "|"}
					}, window \
				) \
			), "other");
		}

		void paintEvent( \
			QPaintEvent *event \
		) override {
			_tabBar.set_style();
		}
	};



	//Main TabWidget

	class MainTabWidget(QTabWidget):
	private:
		CustomTabBar _tabBar;
	public:
		explicit MainTabWidget( \
			Window *window \
		) : QTabWidget() {
			_tabBar = CustomTabBar(this, window);
			setTabBar(_tabBar);  // Устанавливаем кастомный TabBar
			addTab(TabQWidget( \
				GridBasicCalc(window) \
			), "Basic");
			addTab(TabQWidget( \
				GridIntegralCalc(window) \
			), "Integral");
			addTab(TabQWidget( \
				GridDerivativeOrIntegrateCalc(window, 2) \
				), "Derivative");
			addTab(TabQWidget( \
				GridDerivativeOrIntegrateCalc(window, 3) \
			), "Integrate");
			addTab(TabQWidget( \
				GridReplacementCalc(window) \
			), "Replacement");

		void paintEvent( \
			QPaintEvent *event \
		) override {
			_tabBar.set_style();
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
		/*
		addLayout(GridCalculateCommon(window));
		addWidget(TabWidgetKeybord(window));
		*/
	}
};

inline void Window::postInit() {
    setLayout(new MainLayout(_app, this));
}
