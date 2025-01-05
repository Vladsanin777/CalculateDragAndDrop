#include <vector>
#include <tuple>
#include <functional>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <memory>
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
	mutable vector<vector<uintptr_t>> _lineEdit = {{}, {}, {}, {}, {}};
	short _inputtin[2] = {0, 0};
	vector<vector<const char *>> _result = {
		{"0"}, {"1", "2", "0"}, {"0"}, {"0"}, {"x", "0", "0"}
	};
	uintptr_t _resultButton;
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
			short tab, short index \
	) const {
		return _lineEdit.at(tab).at(index);
	}
	void setLineEdit( \
			short tab, uintptr_t newLineEdit \
	) const {
		_lineEdit.at(tab).push_back(newLineEdit);
	}
	void setInputtin(short inputtin[2]) {
		_inputtin[0] = inputtin[0];
		_inputtin[1] = inputtin[1];
	}
	uintptr_t getResultButton() {
		return _resultButton;
	}
	void setResultButton( \
		uintptr_t resultButton \
	) {
		_resultButton = resultButton;
		return;
	}
	const char *getResult( \
		short tab, short index \
	) {
		return _result.at(tab).at(index);
	}
	void setResult( \
		short tab, short index, \
		const char *newResult
	) {
		_result.at(tab).at(index) = \
			newResult;
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
				short inputtin[2], const char *text = "" \
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
			const char *label, Window *window, short fontSize, \
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
			const char *label, Window *window, short fontSize, \
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
			const char *label, Window *window, short fontSize, \
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
	class BuildingGridKeyboard {
	public:
		explicit BuildingGridKeyboard( \
			vector<vector<const char *>> buttons, \
			QGridLayout *grid, \
			Window *window, short row = short(0), \
			std::function<QWidget*( \
				const char*, function<void(QPushButton *)> *, Window* \
			)> createButton = []( \
				const char* label, \
				function<void(QPushButton *)> *callback, \
				Window *window \
			) -> QWidget* {
				return static_cast<QWidget *>( \
					new Button::ButtonDrag( \
						label, window, 20, callback \
					) \
				);
			} \
		) {
			short buttonsLenght = buttons.size();
			for ( \
				short index = short(0); \
				index != buttonsLenght; index++ \
			) {
				short column = short(0);
				vector<const char *> rowLabelsButton = buttons.at(index);
				for ( \
					short rowIndex = short(0); \
					rowIndex != buttonsLenght; rowIndex++ \
				) {
					const char *labelButton = rowLabelsButton.at(rowIndex);
					grid->addWidget(createButton( \
						labelButton, \
						&LogicCalculate::inputing_line_edit, \
						window \
					), row, column, 1, 1);
					column++;
				}
				row++;
			}
		}
	};

	class GridCalculateKeyboard : public QGridLayout {
	public:
		explicit GridCalculateKeyboard( \
			vector<vector<const char *>> buttons, \
			Window *window
		) {
			setContentsMargins(0, 0, 0, 0);
			setSpacing(0);
			BuildingGridKeyboard( \
				buttons, \
				static_cast<QGridLayout *>(this), \
				window \
			);
		}
	};


	class GridCalculateCommon : public QGridLayout {
	private:
		Window *_window = nullptr;
	public:
		explicit GridCalculateCommon( \
			Window *window
		) {
			setSpacing(0);
			setContentsMargins(0, 0, 0, 0);

			_window = window;

			createButton("_ALL", 0, 0);
			createButton("_DO", 0, 1);
			createButton("_RES", 0, 2);
			createButton("_POS", 0, 3);
			createButton("_O", 0, 4);

			BuildingGridKeyboard( \
				vector<vector<const char *>>{
					{"()", "(", ")", "mod", "_PI"}, 
					{"7", "8", "9", ":", "sqrt"}, 
					{"4", "5", "6", "*", "^"}, 
					{"1", "2", "3", "-", "!"}, 
					{"0", ".", "%", "+", "_E"},
					{"", "", "", "", ""}
				}, this, window, 1, \
				[]( \
					const char* label, \
					function<void(QPushButton *)> *callback, \
					Window *window \
				) -> QWidget* {
					return static_cast<QWidget *>( \
						new Button::ButtonDragAndDrop( \
							label, window, 20, callback \
						) \
					);
				} \
			);
			Button::ButtonDrag *resultButton = \
				nullptr;
			window->setResultButton( \
				reinterpret_cast<uintptr_t>(
					resultButton = \
						new Button::ButtonDrag( \
							window->getResult(0, 0), \
							window, 20 \
						) \
				) \
			);
			addWidget( \
				resultButton, \
				7, 0, 1, 5 \
			);
		}

		void createButton( \
			const char *label, short row, \
			short column, \
			function<QWidget *( \
				const char *, short, Window *, \
				function<void(QPushButton *)> *, const char * \
			)> creatorButton = []( \
				const char *label, \
				short fontSize, \
				Window *window, \
				function<void(QPushButton *)> *callback, \
				const char *cssName \
			) {
				return static_cast<QWidget *>( \
					new Button::ButtonDrag( \
						label, window, fontSize, \
						callback, cssName \
					) \
				);
			} \
		) {
			addWidget(creatorButton( \
				label, 20, _window, \
				&LogicCalculate::inputing_line_edit, \
				"keybord" \
			), row, column, 1, 1);
		}
	};

	class GridBaseCalc : public QGridLayout {
	public:
		explicit GridBaseCalc( \
			Window *window \
		) : QGridLayout() {
			setSpacing(0);
			setContentsMargins(0, 0, 0, 0);
			CreateHistori::HistoriScroll *localHistori = nullptr;
			window->setLocalHistori( \
				reinterpret_cast<uintptr_t>( \
					localHistori = \
						new CreateHistori::HistoriScroll()
				) \
			);
			CreateHistori::HistoriWidget *resizeLocalHistori = nullptr;
			window->setResizeLocalHistori( \
				reinterpret_cast<uintptr_t>(
					resizeLocalHistori = \
						localHistori->getResizeHistori() \
				) \
			);
			window->setAddLocalHistori( \
				reinterpret_cast<uintptr_t>( \
					resizeLocalHistori->getAddHistori() \
				) \
			);
			addWidget(localHistori, 0, 0, 1, 6);
		}
	};

	class GridBasicCalc : public GridBaseCalc {
	public:
		explicit GridBasicCalc( \
			Window *window \
		) : GridBaseCalc(window) {
			short input[2] = {0, 0};
			LineEdit::LineEdit *lineEdit = \
				new LineEdit::LineEdit(window, input);
			window->setLineEdit( \
				0, reinterpret_cast<uintptr_t>(lineEdit) \
			);
			addWidget(lineEdit);
		}
	};


	class GridIntegralCalc : public GridBaseCalc {
	public:
		explicit GridBasicCalc( \
			Window *window \
		) : GridBaseCalc(window) {
			self.addWidget( \
				Button::ButtonBase( \
					"a = ", 20, window, \
					nullptr, "calculate" \
				), 1, 0, 1, 1 \
			);
			LineEdit *aLineEdit = \
				new LineEdit(window, {1, 0}, "1");
			window->setLineEdit(1, aLineEdit);
			addWidget(aLineEdit, 1, 1, 1, 2);
			addWidget( \
				Button::ButtonBase( \
					"b = ", 20, window, \
					nullptr, "calculate", \
				), 1, 3, 1, 1 \
			);
			LineEdit *bLineEdit = \
				new LineEdit(window, {1, 1}, "2");
			window->setLineEdit(1, b_line_edit);
			addWidget(bLineEdit, 1, 4, 1, 2);
			LineEdit::LineEdit *mainLineEdit = \
				new LineEdit::LineEdit(window, {1, 2});
			window->line_edit(1, mainLineEdit);
			addWidget(mainLineEdit, 2, 0, 1, 6);
		}
	};

	class GridDerivativeOrIntegrateCalc : public GridBaseCalc {
	public:
		explicit GridDerivativeOrIntegrateCalc( \
			Window *window, short numberTab \
		) : GridBaseCalc(window) {
			short input[2] = {numberTab, 0};
			LineEdit::LineEdit *lineEdit = \
				new LineEdit::LineEdit(window, input);
			window->setLineEdit( \
				numberTab, \
				reinterpret_cast<uintptr_t>(lineEdit) \
			);
			addWidget(lineEdit);
		}
	};
	class GridReplacementCalc : public GridBaseCalc {
	public:
		explicit GridReplacementCalc( \
			Window *window \
		) : GridBaseCalc(window) {
			addWidget( \
				Button::ButtonBase( \
					"with =", 20, window, \
					nullptr, "calculate" \
				), 1, 0, 1, 1 \
			);
			LineEdit *withLineEdit = \
				new LineEdit(window, {4, 0}, "x");
			window->setLineEdit(4, withLineEdit);
			addWidget(withLineEdit, 1, 1, 1, 2);
			addWidget( \
				Button::ButtonBase( \
					"on =", css_name = "calculate", \
					width = 100, window = window \
				), 1, 3, 1, 1 \
			);
			LineEdit *onLineEdit = \
				new LineEdit(window, (4, 1), "0");
			window->setLineEdit(4, onLineEdit);
			addWidget(onLineEdit, 1, 4, 1, 2);
			LineEdit *mainLineEdit = \
				new LineEdit(window, (4, 2));
			window->setLineEdit(4, mainLineEdit);
			addWidget(mainLineEdit, 2, 0, 1, 6);
		}
	};
}



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
		window->setGlobalHistori( \
			reinterpret_cast<uintptr_t>( \
				globalHistori = new HistoriScroll() \
			) \
		);
		HistoriWidget *resizeGlobalHistori = nullptr;
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
		addWidget(TabWindow::MainTabWidget(window));
		/*
		addLayout(GridCalculateCommon(window));
		addWidget(TabWidgetKeybord(window));
		*/
	}
};

inline void Window::postInit() {
    setLayout(new MainLayout(_app, this));
}
