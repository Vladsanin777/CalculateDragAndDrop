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
#include <QScrollBar>
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
#include <QTabBar>
#include <QTabWidget>

#define BYTE_MAX 255
#define COUNT_LOCAL_HISTORI 5

using byte = unsigned char;

class CalculateDragAndDrop;
class Window;
class MainLayout;
class LogicCalculate;
namespace GradientFont {
	class Pen;
	class Path;
	class StyleBase;
	class StyleButton;
	class StyleLineEdit;
}
namespace LineEdit {
	class LineEdit;
}
namespace Button {
	class ButtonBase;
	class ButtonDrag;
	class ButtonDragAndDrop;
}
namespace CreateHistori {
	class HistoriVBox;
	class HistoriWidget;
	class HistoriScroll;
}
namespace Title {
	class Action;
	class Menu;
	class TitleLayout;
	class TitleBar;
}
namespace Grid {
	class BuildingGridKeyboard;
	class GridCalculateKeyboard;
	class GridCalculateCommon;
	class GridBaseCalc;
	class GridBasicCalc;
	class GridIntegralCalc;
	class GridDerivateOrIntegrateCalc;
	class GridReplacementCalc;
}
namespace TabWindow {
	class CustomTabBar;
	class TabQWidget;
	class TabWidgetKeyboard;
	class MainTabWidget;
}

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
	CreateHistori::HistoriScroll ** const _localHistori \
		{new CreateHistori::HistoriScroll*[COUNT_LOCAL_HISTORI]{nullptr}};
	CreateHistori::HistoriWidget ** const _resizeLocalHistori \
		{new CreateHistori::HistoriWidget*[COUNT_LOCAL_HISTORI]{nullptr}};
	CreateHistori::HistoriVBox ** const _addLocalHistori \
		{new CreateHistori::HistoriVBox*[COUNT_LOCAL_HISTORI]{nullptr}};
	LineEdit::LineEdit ** const * const _lineEdit { \
		new LineEdit::LineEdit ** const [1]{nullptr}, \
		new LineEdit::LineEdit ** const [3]{nullptr, nullptr, nullptr}, \
		new LineEdit::LineEdit ** const [1]{nullptr}, \
		new LineEdit::LineEdit ** const [1]{nullptr}, \
		new LineEdit::LineEdit ** const [3]{nullptr, nullptr, nullptr} \
	}
	const char ** const * const _result { \
		new const char ** const [5]{ \
			{new const char * [1]{"0"}}, \
			{new const char * [3]{"1", "2", "0"}}, \
			{new const char * [1]{"0"}}, \
			{new const char * [1]{"0"}}, \
			{new const char * [3]{"x", "0", "0"}} \
		} \
	};
	byte * const _inputtin {new byte [2]{0, 0}};
	Button::ButtonDrag *_resultButton {nullptr};
	CreateHistori::HistoriScroll *_globalHistori {nullptr};
	CreateHistori::HistoriVBox *_addGlobalHistori {nullptr};
	CreateHistori::HistoriWidget *_resizeGlobalHistori {nullptr};
	CalculateDragAndDrop *_app {nullptr};
public:
	explicit Window( \
			CalculateDragAndDrop *app = nullptr \
	) : QWidget(), _app{app} {
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
                std::vector<std::tuple<float, QColor>>{
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

	CreateHistori::HistoriScroll* getGlobalHistori( \
	) const {
		return _globalHistori;
	}
	void setGlobalHistori( \
		CreateHistori::HistoriScroll* newGlobalHistori \
	) {
		_globalHistori = newGlobalHistori;
		return;
	}	
	CreateHistori::HistoriWidget* getResizeGlobalHistori( \
	) const {
		return _resizeGlobalHistori;
	}
	void setResizeGlobalHistori( \
		CreateHistori::HistoriWidget* newResizeGlobalHistori \
	) {
		_resizeGlobalHistori = newResizeGlobalHistori;
		return;
	}

	CreateHistori::HistoriVBox* getAddGlobalHistori( \
	) const {
		return _addGlobalHistori;
	}
	void setAddGlobalHistori( \
		CreateHistori::HistoriVBox* newAddGlobalHistori \
	) {
		_addGlobalHistori = newAddGlobalHistori;
		return;
	}
	CreateHistori::HistoriScroll* getLocalHistori( \
		byte index \
	) const {
		return _localHistori[index];
	}
	CreateHistori::HistoriScroll* getLocalHistori( \
		void \
	) const {
		return _localHistori[_inputtin[0]];
	}
	void setLocalHistori( \
		CreateHistori::HistoriScroll* newLocalHistori, \
		byte index \
	) {
		_localHistori[index] = newLocalHistori;
		return;
	}	
	CreateHistori::HistoriWidget* getResizeLocalHistori( \
			byte index \
	) const {
		return _resizeLocalHistori[index];
	}
	CreateHistori::HistoriWidget* getResizeLocalHistori( \
		void \
	) const {
		return _resizeLocalHistori[_inputtin[0]];
	}
	void setResizeLocalHistori( \
		CreateHistori::HistoriWidget* newResizeLocalHistori, \
		byte index \
	) {
		_resizeLocalHistori[index] = newResizeLocalHistori;
		return;
	}

	CreateHistori::HistoriVBox* getAddLocalHistori( \
		byte index \
	) const {
		return _addLocalHistori[index];
	}
	CreateHistori::HistoriVBox* getAddLocalHistori( \
		void \
	) const {
		return _addLocalHistori[_inputtin[0]];
	}
	void setAddLocalHistori( \
		CreateHistori::HistoriVBox* newAddLocalHistori, \
		byte index
	) {
		_addLocalHistori[index] = newAddLocalHistori;
		return;
	}
	LineEdit::LineEdit* getLineEdit( \
		byte tab, byte index \
	) const {
		return _lineEdit[tab][index];
	}
	LineEdit::LineEdit* getLineEdit(void) const {
		return _lineEdit[*_inputtin][_inputtin[1]];
	}
	void setLineEdit( \
			byte tab, byte index, LineEdit::LineEdit* newLineEdit \
	) {
		_lineEdit[tab][index] = newLineEdit;
		return;
	}
	byte* getInputtin() const {
		return _inputtin;
	}
	void setInputtin(const byte &tab, const byte &index) {
		*_inputtin = tab;
		_inputtin[1] = index;
		return;
	}
	Button::ButtonDrag* getResultButton() {
		return _resultButton;
	}
	void setResultButton( \
		Button::ButtonDrag* resultButton \
	) {
		_resultButton = resultButton;
		return;
	}
	const char *getResult( \
		byte tab, byte index \
	) const {
		return _result[tab][index];
	}
	const char *getResult(void) const {
		return _result[*_inputtin][_inputtin[1]];
	}
	void setResult( \
		const char *newResult,
		byte tab, byte index \
	) {
		const char * & res {_result[tab][index]};
		delete[] res;
		res = newResult;
		return;
	}
	void setResult( \
		const char *newResult \
	) {
		const char * & res {_result[*_inputtin][_inputtin[1]]};
		delete [] res;
		res = newResult;
		return;
	}
	~Window(void) {

		return;
	}
};


inline void CalculateDragAndDrop::createWindow( \
	QPushButton * button \
) {
	Window *win = new Window(this);
	win->postInit();
	win->show();
}


class LogicCalculate {
private:
    char *_lineEditText;
	Window *_window = nullptr;
public:
	explicit LogicCalculate( \
		char *lineEditText, \
		Window *window \
	) : _lineEditText(lineEditText), \
	_window(window) {}

	void button_ALL() {
		char* pos;

		// Удаляем "_ALL" из строки
		pos = strstr(_lineEditText, "_ALL");
		memmove(pos, pos + 4, strlen(pos + 4) + 1); // Сдвигаем остаток строки влево

		if (strlen(_lineEditText) > 0) {
			// window->setResult(_lineEditText);  // Устанавливаем результат
			
			buttonOther();                         // Вызываем другую функцию
			addHistori();    // Добавляем в историю

			_window->setResult("0");            // Сбрасываем результат
		}

		reinterpret_cast<QLineEdit *>( \
			_window->getLineEdit() \
		)->setText(""); // Очищаем поле ввода
	}

	void button_DO() {
		char* pos;
		pos = strstr(_lineEditText, "_DO");
		memmove(pos, pos + 3, strlen(pos + 3) + 1);

		if (strlen(_lineEditText) > 0) {
			// _window->setResult(_lineEditText);
			buttonOther();
			*pos = '\0';
			addHistori();
		}
		reinterpret_cast<QLineEdit *>(
			_window->getLineEdit() \
		)->setText(_lineEditText);
	}

	void button_POS() {
		char* pos;
		// Удаляем "_POS" из строки
		pos = strstr(_lineEditText, "_POS");
		memmove(pos, pos + 4, strlen(pos + 4) + 1); // Сдвигаем остаток строки влево

		if (strlen(_lineEditText) > 0) {
			// _window->setResult(_lineEditText);
			buttonOther();
			pos += strlen("_POS");
			memmove(_lineEditText, pos, strlen(pos) + 1);
			addHistori();
		}
		reinterpret_cast<QLineEdit *>(
			_window->getLineEdit() \
		)->setText(pos);
	}

	void button_RES() {


        const char *result = _window->getResult();
		char *pos = strstr(_lineEditText, "_RES");
		memmove(pos, pos + 4, strlen(pos + 4) + 1); // Сдвигаем остаток строки влево

		if (strlen(_lineEditText) > 0) {
            // window.setResult(_lineEditText);
            buttonOther();
            addHistori();
		}
        QLineEdit *line_edit = reinterpret_cast<QLineEdit *>( \
			_window->getLineEdit() \
		);
        line_edit->setText(result);
        line_edit->setCursorPosition(strlen(result)-1);
	}

	void addHistori() {

		QLayout *element{0L};
		short tab = _window->getInputtin()[0];

		switch (tab) {
			case 1:
				element = new CustomBoxHistoriElement(
					_lineEditText, _window, 
					1, "Integral",
					"a", window->getResult(1, 0),
					"b", window->getResult(1, 1)
				);
				break;

			case 4:
				element = new CustomBoxHistoriElement(
					_lineEditText, _window, 
					4, "Replacement",
					"with", window->getResult(4, 0),
					"on", window->getResult(4, 1)
				);
				break;

			default:
				{
					std::map<short, const char*> lstTabs = {
						{0, "Basic"}, {2, "Derivate"}, {3, "Integrate"}
					};
					element = new BasicBoxHistoriElement(
						line_edit_text, window,
						window->result, lst_tabs[tab]
					);
				}
				break;
		}


		// Добавление элемента в глобальную историю
		reinterpret_cast<QVBoxLayout *>(
			_window->getAddGlobalHistori()
		)->addLayout(element);
		reinterpret_cast<QWidget *>(
			_window->getResizeGlobalHistori()
		)->adjustSize();
		QScrollArea *globalHistori = \
			reinterpret_cast<QScrollArea *>(
				_window->getGlobalHistori()
			);
		globalHistori->verticalScrollBar()->setValue(
			globalHistori->verticalScrollBar()->maximum()
		);

		// Добавление элемента в локальную историю
		reinterpret_cast<QVBoxLayout *>(
			_window->getAddLocalHistori()
		)->addLayout(element);
		reinterpret_cast<QWidget *>(
			_window->getResizeLocalHistori()
		)->adjustSize();
		QScrollArea *localHistori = \
			reinterpret_cast<QScrollArea *>(
				_window->getLocalHistori()
			);
		localHistori->verticalScrollBar()->setValue(
			localHistori->verticalScrollBar()->maximum()
		);
	}

    void button_O() {
		char *pos = strstr(_lineEditText, "_O");
		memmove(pos-1, pos + 3, strlen(pos + 3) + 1);

		reinterpret_cast<QLineEdit *>(
			_window->getLineEdit()
		)->setText(
			_lineEditText
		);
	}

	
	void buttonOther() {
		auto integral = [_window = this->_window]() {
			char* equation = strdup( \
				reinterpret_cast<QLineEdit *>(
					_window->getLineEdit(1, 2)
				)->text().toUtf8().data() \
			);
			// _window->setResult(equation, 1, 2);
		};

		auto otherTab = [_window = this->_window]() {
			// _window->setResult(_window->getLineEdit(), 0, 0);
		};
		array<short, 2> inputtin = _window->getInputtin();
		switch (inputtin[0] * 10 + inputtin[1]) {
			case 10:
			case 11:
				otherTab();
			case 12:
				integral();
				break;
			case 20:
				//_window->setResult({0, 0}, window.line_edit_text); // Пример для Derivative
				break;
			case 30:
				// _window.setResult({0, 0}, window.line_edit_text); // Пример для Derivative с true
				break;
			case 40:
			case 41:
				// _window.setResult({0, 0}, window.line_edit_text);
				break;
			case 42: {
				// char* modified_text = replaceAll(window.line_edit_text, window.getResult(4, 0), window.getResult(4, 1));
				// _window.setResult({0, 0}, modified_text);
				break;
			}
			default:
				otherTab();
				break;
		}

		// _window.setForResult(window.result);
	}
            
	static void inputtinLineEdit( \
		QPushButton *button, Window *window \
	) {
		char *label = strdup(button->text().toUtf8().data());
		QLineEdit *lineEdit = reinterpret_cast<QLineEdit *>( \
				window->getLineEdit() \
		);
		char *text = strdup(lineEdit->text().toUtf8().data());
		short positionCursor = lineEdit->cursorPosition();
		char *result;
		// Копируем часть строки до курсора
		strncpy(result, text, positionCursor);
		result[positionCursor] = '\0'; // Завершаем строку

		// Добавляем label
		strcat(result, label);

		// Добавляем оставшуюся часть строки после курсора
		strcat(result, text + positionCursor);
        lineEdit->setText(result);
        lineEdit->setCursorPosition(positionCursor + strlen(label));
	}
};


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
		array<short, 2> _inputtin;
	public:
		explicit LineEdit ( \
				Window *window, \
				array<short, 2> inputtin, const char *text = "" \
		) : _window(window), QLineEdit() {
			_inputtin = inputtin;
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
						&LogicCalculate::inputinLineEdit, \
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
				&LogicCalculate::inputinLineEdit, \
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
		explicit GridIntegralCalc( \
			Window *window \
		) : GridBaseCalc(window) {
			addWidget( \
				new Button::ButtonBase( \
					"a = ", window, 20, \
					nullptr, "calculate" \
				), 1, 0, 1, 1 \
			);
			short input[2] = {1, 0};
			LineEdit::LineEdit *aLineEdit = \
				new LineEdit::LineEdit(window, input, "1");
			window->setLineEdit( \
				1, reinterpret_cast<uintptr_t>( \
					aLineEdit \
				) \
			);
			addWidget(aLineEdit, 1, 1, 1, 2);
			addWidget( \
				new Button::ButtonBase( \
					"b = ", window, 20, \
					nullptr, "calculate" \
				), 1, 3, 1, 1 \
			);
			input[0] = 1, input[1] = 1;
			LineEdit::LineEdit *bLineEdit = \
				new LineEdit::LineEdit(window, input, "2");
			window->setLineEdit( \
				1, reinterpret_cast<uintptr_t>( \
					bLineEdit \
				) \
			);
			addWidget(bLineEdit, 1, 4, 1, 2);
			input[0] = 1, input[1] = 2;
			LineEdit::LineEdit *mainLineEdit = \
				new LineEdit::LineEdit(window, input);
			window->setLineEdit( \
				1, reinterpret_cast<uintptr_t>( \
					mainLineEdit \
				) \
			);
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
				new Button::ButtonBase( \
					"with =", window, 20, \
					nullptr, "calculate" \
				), 1, 0, 1, 1 \
			);
			short input[2] = {4, 0};
			LineEdit::LineEdit *withLineEdit = \
				new LineEdit::LineEdit(window, input, "x");
			window->setLineEdit( \
				4, reinterpret_cast<uintptr_t>( \
					withLineEdit \
				) \
			);
			addWidget(withLineEdit, 1, 1, 1, 2);
			addWidget( \
				new Button::ButtonBase( \
					"on =", window, 20, \
					nullptr, "calculate" \
				), 1, 3, 1, 1 \
			);
			input[1] = 1;
			LineEdit::LineEdit *onLineEdit = \
				new LineEdit::LineEdit(window, input, "0");
			window->setLineEdit( \
				4, reinterpret_cast<uintptr_t>( \
					onLineEdit \
				) \
			);
			addWidget(onLineEdit, 1, 4, 1, 2);
			input[1] = 2;
			LineEdit::LineEdit *mainLineEdit = \
				new LineEdit::LineEdit(window, input);
			window->setLineEdit( \
				4, reinterpret_cast<uintptr_t>( \
					mainLineEdit \
				) \
			);
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

		void createStyle() {
			// Задаем градиенты
			_gradient = new CreateGradient(_tabWidget, _window);
		}
		void paintEvent(QPaintEvent *event) override {
			QPainter *painter = new QPainter(this);
			painter->setRenderHint(QPainter::RenderHint::Antialiasing);
			short countTabs = static_cast<short>(count());
			for (int index; index != count(); index++) {
				QRect rect = tabRect(index);

				// Установка шрифта
				QFont fontTabBar = font();
				painter->setFont(fontTabBar);

				// Получение текста вкладки
				QString text = tabText(index);
				QFontMetrics *metrics = new QFontMetrics(fontTabBar);
				short textWidth = metrics->horizontalAdvance(text);
				short textHeight = metrics->height();

				// Центрирование текста внутри вкладки
				short x = rect.x() + (rect.width() - textWidth) / 2;
				short y = rect.y() + (rect.height() + textHeight) \
					/ 2 - metrics->descent();

				// Создаём путь текста
				QPainterPath path = QPainterPath();
				path.addText(x, y, fontTabBar, text);

				// Рисуем текст разным цветом для активной вкладки
				QPen pen = QPen();
				pen.setColor(QColor("white"));  // Цвет текста неактивной вкладки
				bool isSelected = currentIndex() == index;
				if (isSelected)
					pen.setWidth(4);
				else
					pen.setWidth(2);
				painter->setPen(pen);
				painter->setBrush(Qt::BrushStyle::NoBrush);
				painter->drawPath(path);

				// Установка градиента для вкладки
				painter->setBrush(QBrush(*_gradient));
				painter->setPen(Qt::PenStyle::NoPen);
				painter->drawPath(path);
			}
			painter->end();
		}
	};



	class TabQWidget : public QWidget {
	public:
		explicit TabQWidget( \
			QGridLayout *tab \
		) : QWidget() {
			setLayout(tab);
		}
	};


	class TabWidgetKeyboard : public QTabWidget {
	private:
		Window *_window = nullptr;
		CustomTabBar *_tabBar;
	public:
		explicit TabWidgetKeyboard( \
			Window *window \
		) : QTabWidget() {
			_window = window;
			setFixedHeight(110);
			_tabBar = new CustomTabBar(this, window);
			setTabBar(_tabBar);  // Устанавливаем кастомный TabBar
			addTab(new TabQWidget( \
				new Grid::GridCalculateKeyboard( \
					vector<vector<const char *>>{
						{"1", "2", "3", "4", "5"},
						{"6", "7", "8", "9", "0"}
					}, window \
				) \
			), "digits 10");
			addTab(new TabQWidget( \
				new Grid::GridCalculateKeyboard( \
					vector<vector<const char *>>{
						{"A", "B", "C"},
						{"D", "E", "F"}
					}, window \
				) \
			), "digits 16");
			addTab(new TabQWidget( \
				new Grid::GridCalculateKeyboard( \
					vector<vector<const char *>>{
						{"+", "-", ":", "*", "^"},
						{"!", "sqrt", "ln", "log", "lg"}
					}, window \
				) \
			), "operators");
			addTab(new TabQWidget( \
				new Grid::GridCalculateKeyboard( \
					vector<vector<const char *>>{
						{"_E", "_PI"}
					}, window \
				) \
			), "consts");
			addTab(new TabQWidget( \
				new Grid::GridCalculateKeyboard( \
					vector<vector<const char *>>{
						{"sin(", "cos(", "tan("},
						{"sec(", "csc(", "cot("}
					}, window \
				) \
			), "trigonometric functions");
			addTab(new TabQWidget( \
				new Grid::GridCalculateKeyboard( \
					vector<vector<const char *>>{
						{"sgn(", "abs(", "mod"}
					}, window \
				) \
			), "other functions");
			addTab(new TabQWidget( \
				new Grid::GridCalculateKeyboard( \
					vector<vector<const char *>>{
						{"0x", "0b", "0t"}
					}, window \
				) \
			), "number system");
			addTab(new TabQWidget( \
				new Grid::GridCalculateKeyboard( \
					vector<vector<const char *>>{
						{"%", "mod", ".", "|"}
					}, window \
				) \
			), "other");
		}

		void paintEvent( \
			QPaintEvent *event \
		) override {
			_tabBar->createStyle();
		}
	};



	//Main TabWidget

	class MainTabWidget : public QTabWidget {
	private:
		CustomTabBar *_tabBar;
	public:
		explicit MainTabWidget( \
			Window *window \
		) : QTabWidget() {
			_tabBar = new CustomTabBar(this, window);
			setTabBar(_tabBar);  // Устанавливаем кастомный TabBar
			addTab(new TabQWidget( \
				new Grid::GridBasicCalc(window) \
			), "Basic");
			addTab(new TabQWidget( \
				new Grid::GridIntegralCalc(window) \
			), "Integral");
			addTab(new TabQWidget( \
				new Grid::GridDerivativeOrIntegrateCalc(window, 2) \
				), "Derivative");
			addTab(new TabQWidget( \
				new Grid::GridDerivativeOrIntegrateCalc(window, 3) \
			), "Integrate");
			addTab(new TabQWidget( \
				new Grid::GridReplacementCalc(window) \
			), "Replacement");
		}

		void paintEvent( \
			QPaintEvent *event \
		) override {
			_tabBar->createStyle();
		}
	};
}


class MainLayout : public QVBoxLayout {
public:
	explicit MainLayout( \
		CalculateDragAndDrop *app, Window *window \
	) : QVBoxLayout() {
		setContentsMargins(0, 0, 0, 0);
		setSpacing(0);
		addWidget(new Title::TitleBar(app, window));
		CreateHistori::HistoriScroll *globalHistori = nullptr;
		window->setGlobalHistori( \
			reinterpret_cast<uintptr_t>( \
				globalHistori = new CreateHistori::HistoriScroll() \
			) \
		);
		CreateHistori::HistoriWidget *resizeGlobalHistori = nullptr;
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
		addWidget(new TabWindow::MainTabWidget(window));
		addLayout(new Grid::GridCalculateCommon(window));
		addWidget(new TabWindow::TabWidgetKeyboard(window));
	}
};

inline void Window::postInit() {
    setLayout(new MainLayout(_app, this));
}
