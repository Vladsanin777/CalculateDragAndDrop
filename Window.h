#include <functional>
#include <cstring>
#include <iostream>
#include <QPushButton>
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QPointF>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QLineEdit>
#include <QString>
#include <QFocusEvent>
#include <QDrag>
#include <QMimeData>
#include <QTabBar>
#include <QTabWidget>
#include <QLabel>
#include <QStyle>
#include "Calculate.h"

#define COUNT_LOCAL_HISTORI 5

using byte = unsigned char;

class CalculateDragAndDrop;
class Window;

namespace Button {
	class ButtonBase;
	class ButtonDrag;
	class ButtonDragAndDrop;
}
class LineEdit;
namespace NewHistoriElement {
	class LabelHistori;
	class BaseBoxHistoriElement;
	class BasicBoxHistoriElement;
	class SubCustomBoxHistoriElement;
	class CustomBoxHistoriElement;
}
namespace CreateHistori {
	class HistoriVBox;
	class HistoriWidget;
	class HistoriScroll;
}
class LogicCalculate;
namespace Title {
	class TitleLayout;
	class TitleBar;
}
namespace Grid {
	template<class TButton, byte rowEnd, byte columnEnd>
	void buildingGridKeyboard( \
		const char * const (&buttons)[rowEnd][columnEnd], \
		QGridLayout *grid, Window *window, \
		const byte rowStart = byte(0), \
		const byte columnStart = byte(0), \
		const char * cssName = "opertor" \
	) noexcept;
	template<class TButton, byte rowEnd, byte columnEnd>
	class GridCalculateKeyboard;
	class GridCalculateCommon;
	class GridBaseCalc;
	class GridDefaultCalc;
	class GridIntegralCalc;
	class GridReplacementCalc;
}
namespace TabWindow {
	class TabQWidget;
	class TabWidgetKeyboard;
	class MainTabWidget;
}
class MainLayout;
class MainWidget;

const char * const windowTitle \
	{"CalculateDragAndDrop"};

const char * const lstTabs[5] {
	"Basic", "Derivate", "Integrate", \
	"Intergal", "Replacement" \
};
const byte rowDigits10 {byte(2)}, \
	columnDigits10 {byte(5)};
const char * const digits10[rowDigits10][columnDigits10] {
	{"0", "1", "2", "3", "4"}, \
	{"5", "6", "7", "8", "9"} \
};
const byte rowDigits16 {byte(2)}, \
	columnDigits16 {byte(3)};
const char * const digits16[rowDigits16][columnDigits16] {
	{"A", "B", "C"}, \
	{"D", "E", "F"} \
};
const byte rowOperators1 {byte(2)}, \
	columnOperators1 {byte(5)};
const char * const operators1[rowOperators1][columnOperators1] {
	{"+", "-", ":", "*", "^"}, \
	{"!", "sqrt(", "ln(", "log(", "lg("} \
};
const byte rowOperators2 {byte(2)}, \
	columnOperators2 {byte(3)};
const char * const operators2[rowOperators2][columnOperators2] {
	{"sgn(", "abs(", "mod"}, \
	{"exp(", "sqr(", "cbrt("} \
};
const byte rowTrigonometricFuncs {byte(2)}, \
	columnTrigonometricFuncs {byte(3)};
const char * const trigonometricFuncs[rowTrigonometricFuncs] \
	[columnTrigonometricFuncs] {
	{"sin(", "cos(", "tan("}, \
	{"sec(", "csc(", "cot("} \
};
const byte rowBrackets {byte(2)}, \
	columnBrackets {byte(3)};
const char * const brackets[rowBrackets][columnBrackets]{
	{"()", "(", ")"}, \
	{"[]", "[", "]"} \
};
const byte rowBraces {byte(1)}, \
	columnBraces {byte(3)};
const char * const braces[rowBraces][columnBraces] {
	{"{}", "{", "}"} \
};
const byte rowConsts {byte(1)}, \
	columnConsts {byte(2)};
const char * const consts[rowConsts][columnConsts] {
	{"_E", "_PI"} \
};
const byte rowInverseTrigonometricFuncs {byte(2)}, \
	columnInverseTrigonometricFuncs {byte(3)};
const char * const inverseTrigonometricFuncs \
	[rowInverseTrigonometricFuncs] \
	[columnInverseTrigonometricFuncs] {
	{"asin(", "acos(", "atan("}, \
	{"asec(", "acsc(", "acot("} \
};
const byte rowPrefixes {byte(1)}, \
	columnPrefixes {byte(3)};
const char * const prefixes[rowPrefixes][columnPrefixes] {
	{"0b", "0o", "0x"} \
};
const byte rowFunctions{byte(1)}, \
	columnFunctions{byte(5)};
const char * const functions[rowFunctions] \
	[columnFunctions] {
	{"_ALL", "_DO", "_RES", "_POS", "_O"} \
};
const byte rowBracketsMain{byte(1)}, \
	columnBracketsMain{byte(3)};
const char * const bracketsMain[rowBracketsMain] \
	[columnBracketsMain] {
	{"()", "(", ")"} \
};
const byte rowNumbersMain{byte(4)}, \
	columnNumbersMain{byte(3)};
const char * const numbersMain[rowNumbersMain] \
	[columnNumbersMain] {
	{"7", "8", "9"}, \
	{"4", "5", "6"}, \
	{"1", "2", "3"}, \
	{"0", ".", "%"} \
};
const byte rowOperatorsMain{byte(4)}, \
	columnOperatorsMain{byte(2)};
const char * const operatorsMain[rowOperatorsMain] \
	[columnOperatorsMain] {
	{"mod", "sqrt("}, \
	{"*", ":"}, \
	{"+", "-"}, \
	{"^", "!"} \
};
const byte rowConstsMain{byte(1)}, \
	columnConstsMain{byte(2)};
const char * const constsMain[rowConstsMain] \
	[columnConstsMain] {
	{"_PI", "_E"} \
};
const byte rowEmptyMain{byte(1)}, \
	columnEmptyMain{byte(5)};
const char * const emptyMain[rowEmptyMain] \
	[columnEmptyMain] {
	{"", "", "", "", ""} \
};

class CalculateDragAndDrop : public QApplication {
public:
	explicit CalculateDragAndDrop( \
			int argc, char *argv[] \
	) : QApplication(argc, argv) {
		setStyleSheet(R"(
			#function {
				background-color: rgb(204, 69, 50);
			}
			#empty {
				background-color: rgb(107, 110, 195);
			}
			#const {
				background-color: rgb(94, 142, 35);
			}
			#operator {
				background-color: rgb(150, 145, 37);
			}
			#number {
				background-color: rgb(34, 71, 171);
			}
			#bracket {
				background-color: rgb(106, 35, 169);
			}
			#prefix {
				background-color: rgb(94, 0, 0);
			}
			#expression {
				background: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 1,
					stop: 0 rgb(34, 71, 171), stop: 0.75 rgb(106, 35, 169));
			}
			#function:hover,
			#empty:hover,
			#const:hover,
			#operator:hover,
			#number:hover,
			#bracket:hover,
			#prefix:hover,
			#expression:hover {
				background-color: #777;
				border: 5px solid white;
			}
			#title {
				padding: 5px 15px;
			}
			QLineEdit {
				background: transparent;
				text-align: center;
				color: white;
				margin: 0;
			}
			#hide {
				margin: 0 10px 10px 10px;
			}
			QPushButton {
				color: white;
				border-radius: 10px;
				padding: 4px;
				border: 1px solid rgba(255, 255, 255, 0.5);
				font-size: 20px;
				text-align: start;
			}
			QGridLayout {
				background: transparent;
			}
			QWidget {
				background: transparent;
			}
			QMainWindow {
				background-color: rgb(9, 10, 25);
			}
			QTabWidget {
				background: transparent;
				border: none;
			}
			QTabWidget::pane {
				border: 0;
				background: transparent;
				margin: 0;
				padding: 0;
			}
			
			QTabBar {
				width: 100%;
				background: transparent;
				border: none;
				margin: 0;
				padding: 0;
			}
			
			QTabBar::tab, QLineEdit, #basic, #hide {
				border: 1px solid white;
				border-radius: 10px;
			}
			QTabBar::tab {
				padding: 8px 12px;
				margin: 5px;
				color: white;
				background-color: rgb(107, 110, 195);
				min-width: 80px;
				text-align: start;
				font-size: 15px;
			}
			
			QTabBar::tab:hover {
				background: #666;
				border: none;
			}
			
			QTabBar::tab:selected {
				background: #777;
				font-size: 20px;
				border: 5px solid white;
			}
			QTabWidget::tab-bar {
				alignment: center;
			}
		)");
		createWindow(nullptr);
	}
	void createWindow(QPushButton *button);
};

class Window : public QMainWindow {
private:
	CreateHistori::HistoriScroll ** const _localHistori \
		{new CreateHistori::HistoriScroll*[COUNT_LOCAL_HISTORI]{nullptr}};
	CreateHistori::HistoriWidget ** const _resizeLocalHistori \
		{new CreateHistori::HistoriWidget*[COUNT_LOCAL_HISTORI]{nullptr}};
	CreateHistori::HistoriVBox ** const _addLocalHistori \
		{new CreateHistori::HistoriVBox*[COUNT_LOCAL_HISTORI]{nullptr}};
	LineEdit ** const * const _lineEdit { \
		new LineEdit ** const [COUNT_LOCAL_HISTORI] { \
			new LineEdit * [1]{nullptr}, \
			new LineEdit * [3]{nullptr, nullptr, nullptr}, \
			new LineEdit * [1]{nullptr}, \
			new LineEdit * [1]{nullptr}, \
			new LineEdit * [3]{nullptr, nullptr, nullptr} \
		}
	};
	const char ** const * const _result { \
		new const char ** const [5]{ \
			{new const char * [1]{new const char [] {"0"}}}, \
			{new const char * [3]{new const char [] {"1"}, \
				new const char [] {"2"}, new const char [] {"0"}}}, \
			{new const char * [1]{new const char [] {"0"}}}, \
			{new const char * [1]{new const char [] {"0"}}}, \
			{new const char * [3]{new const char [] {"x"}, \
				new const char [] {"0"}, new const char [] {"0"}}} \
		} \
	};
	byte * const _inputtin {new byte [2]{0, 0}};
	Button::ButtonDrag *_resultButton {nullptr};
	CreateHistori::HistoriScroll *_globalHistori {nullptr};
	CreateHistori::HistoriVBox *_addGlobalHistori {nullptr};
	CreateHistori::HistoriWidget *_resizeGlobalHistori {nullptr};
	CalculateDragAndDrop *_app {nullptr};
public:
	explicit inline Window( \
			CalculateDragAndDrop *app = nullptr \
	) : QMainWindow{}, _app{app} {
		setWindowTitle(::windowTitle);
		resize(400, 800);
		setObjectName("Window");
		show();
		return;
	}

	inline void postInit() noexcept;
	
	inline void changeLanguage(QPushButton *button) noexcept {}

	inline void changeFon(QPushButton *button) noexcept {}

	inline CreateHistori::HistoriScroll* getGlobalHistori( \
	) const noexcept {
		return _globalHistori;
	}
	inline void setGlobalHistori( \
		CreateHistori::HistoriScroll* newGlobalHistori \
	) noexcept {
		_globalHistori = newGlobalHistori;
		return;
	}	
	inline CreateHistori::HistoriWidget* getResizeGlobalHistori( \
	) const noexcept {
		return _resizeGlobalHistori;
	}
	inline void setResizeGlobalHistori( \
		CreateHistori::HistoriWidget* newResizeGlobalHistori \
	) noexcept {
		_resizeGlobalHistori = newResizeGlobalHistori;
		return;
	}

	inline CreateHistori::HistoriVBox* getAddGlobalHistori( \
		void \
	) const noexcept {
		return _addGlobalHistori;
	}
	inline void setAddGlobalHistori( \
		CreateHistori::HistoriVBox* newAddGlobalHistori \
	) noexcept {
		_addGlobalHistori = newAddGlobalHistori;
		return;
	}
	inline CreateHistori::HistoriScroll* getLocalHistori( \
		byte index \
	) const noexcept {
		return _localHistori[index];
	}
	inline CreateHistori::HistoriScroll* getLocalHistori( \
		void \
	) const noexcept {
		return _localHistori[_inputtin[0]];
	}
	inline void setLocalHistori( \
		CreateHistori::HistoriScroll* newLocalHistori, byte tab \
	) noexcept {
		_localHistori[tab] = newLocalHistori;
		return;
	}	
	inline CreateHistori::HistoriWidget* getResizeLocalHistori( \
			byte index \
	) const noexcept {
		return _resizeLocalHistori[index];
	}
	inline CreateHistori::HistoriWidget* getResizeLocalHistori( \
		void \
	) const noexcept {
		return _resizeLocalHistori[_inputtin[0]];
	}
	inline void setResizeLocalHistori( \
		CreateHistori::HistoriWidget* newResizeLocalHistori, \
		byte index \
	) noexcept {
		_resizeLocalHistori[index] = newResizeLocalHistori;
		return;
	}

	inline CreateHistori::HistoriVBox* getAddLocalHistori( \
		byte index \
	) const noexcept {
		return _addLocalHistori[index];
	}
	inline CreateHistori::HistoriVBox* getAddLocalHistori( \
		void \
	) const noexcept {
		std::cout << _inputtin[0] << std::endl;
		return _addLocalHistori[_inputtin[0]];
	}
	inline void setAddLocalHistori( \
		CreateHistori::HistoriVBox* newAddLocalHistori, \
		byte index
	) noexcept {
		_addLocalHistori[index] = newAddLocalHistori;
		return;
	}
	inline LineEdit* getLineEdit( \
		byte tab, byte index \
	) const noexcept {
		return _lineEdit[tab][index];
	}
	inline LineEdit* getLineEdit(void) const noexcept {
		return _lineEdit[*_inputtin][_inputtin[1]];
	}
	inline void setLineEdit( \
		LineEdit* newLineEdit, byte tab, byte index \
	) noexcept {
		std::cout << *_inputtin << _inputtin[1] << std::endl;
		_lineEdit[tab][index] = newLineEdit;
		return;
	}
	inline const byte* getInputtin() const noexcept {
		return _inputtin;
	}
	inline void setInputtin(const byte &tab, const byte &index) noexcept {
		*_inputtin = tab;
		_inputtin[1] = index;
		return;
	}
	inline Button::ButtonDrag* getResultButton() noexcept {
		return _resultButton;
	}
	inline void setResultButton( \
		Button::ButtonDrag* resultButton \
	) noexcept {
		_resultButton = resultButton;
		return;
	}
	inline const char *getResult( \
		byte tab, byte index \
	) const noexcept {
		return _result[tab][index];
	}
	inline const char *getResult(void) const noexcept {
		return _result[*_inputtin][_inputtin[1]];
	}
	inline void setResult( \
		const char *newResult,
		byte tab, byte index \
	) noexcept {
		const char * & res {_result[tab][index]};
		delete[] res;
		res = newResult;
		updataResultButton();
		return;
	}
	inline void setResult( \
		const char *newResult \
	) noexcept {
		const char * & res {_result[*_inputtin][_inputtin[1]]};
		delete [] res;
		res = newResult;
		updataResultButton();
		return;
	}
	template<typename TDel>
	inline void deleteResultOrLineEdit(TDel ptr) const noexcept {
		delete **ptr;
		delete *ptr[1];
		delete ptr[1][1];
		delete ptr[1][2];
		delete *ptr[2];
		delete *ptr[3];
		delete *ptr[4];
		delete ptr[4][1];
		delete ptr[4][2];
		return;
	}
	void updataResultButton(void) const noexcept;
	inline ~Window(void);
};


inline void CalculateDragAndDrop::createWindow( \
	QPushButton * button \
) {
	Window *win = new Window(this);
	win->postInit();
	win->show();
}

namespace Button {
	class ButtonBase : public QPushButton {
	private:
		Window *_window;
	public:
		explicit ButtonBase( \
			const char *label, Window *window, \
			std::function<void(QPushButton *)> *callback = nullptr, \
			const char *cssName = "basic" \
		) : _window(window), QPushButton(label) {
			setAttribute(Qt::WA_Hover, true);
			setContentsMargins(0, 0, 0, 0);
			if (callback) 
				connect(this, &QPushButton::clicked, [this, callback](bool) {
					if (callback) {
						(*callback)(this);
					}
				});
			if (cssName)
				setObjectName(cssName);
			setMinimumWidth(70);
			setMinimumHeight(40);
		}
	};

	class ButtonDrag : public ButtonBase {
	private:
		QPoint _start_pos;
	public:
		explicit ButtonDrag( \
			const char *label, Window *window, \
			std::function<void(QPushButton *)> *callback = nullptr, \
			const char *cssName = nullptr \
		) : ButtonBase( \
			label, window, callback, cssName \
		) {}
	protected:
		void mousePressEvent( \
				QMouseEvent *event \
		) override {
			QPushButton::mousePressEvent(event);
			_start_pos = event->pos();
		}
		void mouseMoveEvent(QMouseEvent *event) override {
			// 1. Сначала всегда вызываем базовую обработку (для hover)
			QPushButton::mouseMoveEvent(event);

			// 2. Проверяем, что зажата именно левая кнопка мыши
			if (!(event->buttons() & Qt::LeftButton))
				return;

			// 3. Увеличиваем порог активации DnD (в 2-3 раза)
			const int dragThreshold = QApplication::startDragDistance() * 2;
			
			// 4. Проверяем расстояние от точки нажатия
			if ((event->pos() - _start_pos).manhattanLength() > dragThreshold) {
				// 5. Создаем DnD операцию
				QDrag* drag = new QDrag(this);
				QMimeData* mimeData = new QMimeData();
				mimeData->setText(text());
				drag->setMimeData(mimeData);
				
				// 6. Запускаем DnD и сохраняем результат
				Qt::DropAction result = drag->exec(Qt::MoveAction);
				
				// 7. После DnD принудительно обновляем состояние кнопки
				if (result != Qt::IgnoreAction) {
					setDown(false);  // Снимаем "нажатое" состояние
					update();        // Перерисовываем кнопку
				}
			}
		}
	};

	class ButtonDragAndDrop : public ButtonDrag {
	public:
		explicit inline ButtonDragAndDrop( \
			const char *label, Window *window, \
			std::function<void(QPushButton *)> *callback = nullptr, \
			const char *cssName = nullptr \
		) : ButtonDrag ( \
			label, window, callback, cssName \
		) {setAcceptDrops(true);}
		
		inline bool isEmpty(const char * &text) {
			return strlen(text) == 0UL;
		}
		inline bool isNumber(const char * &text) {
			size_t len {strlen(text)};
			if (!(isdigit(*text) || ('-' == *text \
				&& len > 1UL)))
				return false;
			if (text[len-1UL] == '%') len--;
			for (const char * ptr {text+1}; --len ; ptr++)
				if (!isdigit(*ptr) && \
					*ptr != '.' && *ptr != ',')
					return false;
			return true;
		}
		inline bool isPrefix(const char * &text) {
			size_t len {strlen(text)};
			if (len < 2) return false;
			if (*text != '0') return false;
			const char second {text[1]};
			if (second != 'b' && \
				second != 'x' && second != 'o')
				return false;
			if (len == 2) return true;
			const char *textlvalue {text+2UL};
			return isNumber(textlvalue);
		}
		inline bool isConst(const char * &text) {
			return strcmp(text, "_E") == 0 || \
				strcmp(text, "_PI") == 0;
		}
		inline bool isBracket(const char * const &text) {
			for (const char * ptr {text}, \
				* const ptrEnd {text+strlen(text)}; \
				ptr != ptrEnd; ptr++) {
				switch (*ptr) {
					case '(':
					case ')':
					case '[':
					case ']':
					case '{':
					case '}':
						continue;
					default:
						return false;
				}
			}
			return true;
		}
		inline bool isFunction(const char * &text) {
			return strcmp(text, "_ALL") == 0 || \
				strcmp(text, "_DO") == 0 || \
				strcmp(text, "_RES")  == 0 || \
				strcmp(text, "_POS") == 0 || \
				strcmp(text, "_O") == 0;
		}
		inline bool is1Operator(const char * text) {
			if (strlen(text) != 1) return false;
			char fristSymbol {*text};
			switch (fristSymbol) {
				case '+':
				case '-':
				case ':':
				case '*':
				case '^':
				case '!':
					return true;
				default:
					return false;
			}
		}
		inline bool is2Operator(const char * text) {
			return *text == 'l' && \
				(*++text == 'n' || *text == 'g') && \
				(*++text == '\0' || isBracket(text));
		}
		inline bool is3Operator(const char * text) {
			if (strcmp(text, "mod") == 0) return true;
			if (memcmp(text, "log", 3UL) == 0 || \
				memcmp(text, "sin", 3UL) == 0 || \
				memcmp(text, "cos", 3UL) == 0 || \
				memcmp(text, "tan", 3UL) == 0 || \
				memcmp(text, "sec", 3UL) == 0 || \
				memcmp(text, "csc", 3UL) == 0 || \
				memcmp(text, "cot", 3UL) == 0 || \
				memcmp(text, "sgn", 3UL) == 0 || \
				memcmp(text, "abs", 3UL) == 0 || \
				memcmp(text, "exp", 3UL) == 0 || \
				memcmp(text, "sqr", 3UL) == 0 \
			) {
				const char *textlvalue {text+3UL};
			 	return isBracket(textlvalue);
			}
			return false;
		}
		inline bool is4Operator(const char * text) {
			if (memcmp(text, "sqrt", 4UL) == 0 || \
				memcmp(text, "asin", 4UL) == 0 || \
				memcmp(text, "acos", 4UL) == 0 || \
				memcmp(text, "atan", 4UL) == 0 || \
				memcmp(text, "asec", 4UL) == 0 || \
				memcmp(text, "acsc", 4UL) == 0 || \
				memcmp(text, "acot", 4UL) == 0 || \
				memcmp(text, "cbrt", 4UL) == 0 \
			) {
				const char *textlvalue {text+4UL};
				return isBracket(text);
			}
			return false;
		}
		inline bool isOperator(const char * &text) {
			if (is1Operator(text)) return true;
			if (is2Operator(text)) return true;
			if (is3Operator(text)) return true;
			if (is4Operator(text)) return true;
			return false;
		}
	protected:
		inline void dragEnterEvent( \
				QDragEnterEvent *event \
		) override {
			QPushButton::dragEnterEvent(event);
			if (event->mimeData()->hasText())
				event->acceptProposedAction();
			return;
		}
		inline void dropEvent( \
				QDropEvent *event \
		) override {
			QPushButton::dropEvent(event);
			const char * text { \
				event->mimeData()->text().toUtf8().constData()}, \
				* cssName {nullptr};
			setText(text);
			if (isEmpty(text)) cssName = "empty";
			else if (isNumber(text)) cssName = "number";
			else if (isPrefix(text)) cssName = "prefix";
			else if (isBracket(text)) cssName = "bracket";
			else if (isConst(text)) cssName = "const";
			else if (isFunction(text)) cssName = "function";
			else if (isOperator(text)) cssName = "operator";
			else cssName = "expression";
			puts(cssName);
			setStyleSheet("");
			setObjectName(cssName);
			style()->unpolish(this);
			style()->polish(this);
			update();
			event->acceptProposedAction();
			return;
		}
	};
}
void Window::updataResultButton(void) const noexcept {
	_resultButton->setText(_result[*_inputtin][_inputtin[1]]);
}

class  LineEdit : public QLineEdit {
private:
	Window *_window;
	byte _tab {0}, _index {0};
	void onLineEditChanged( \
		const QString& text \
	) const noexcept;
public:
	explicit LineEdit ( \
		Window *window, byte tab, byte index, \
		const char *text = "" \
	) : _window{window}, QLineEdit{}, \
	_tab{tab}, _index{index} {
		setDragEnabled(true);
		std::cout << _window << std::endl;
		setText(QString::fromUtf8(text));
		QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		setSizePolicy(sizePolicy);
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
		std::cout << (short)_tab << (short)_index << std::endl;
		_window->setInputtin(_tab, _index);
		QLineEdit::focusInEvent(event);
	}
};
namespace NewHistoriElement {
	class LabelHistori : public QLabel {
	private:
		Window *_window {nullptr};
		const char *_callback {nullptr};
	public:
		explicit inline LabelHistori( \
			const char* label, const char *cssName, \
			Window *window, const char *customCallback = nullptr \
		) : QLabel(label), _callback{customCallback}, \
			_window{window} {
			setAlignment(Qt::AlignmentFlag::AlignCenter);
			setSizePolicy(QSizePolicy::Policy::Expanding, \
				QSizePolicy::Policy::Expanding);
			setContentsMargins(0, 0, 0, 0);
			setObjectName(cssName);
			this->setFont(QFont(this->font().family(), 20));
			if (!customCallback)
				_callback = label;
			else 
				delete [] label;
			return;
		}
	private:
		inline void mousePressEvent(QMouseEvent* event \
		) noexcept override {
			QDrag* drag {new QDrag{this}};
			QMimeData* mimeData {new QMimeData{}};
			mimeData->setText(_callback);
			drag->setMimeData(mimeData);
			drag->exec(Qt::MoveAction);
		}
		~LabelHistori(void) {
			delete [] _callback;
		}
	};
	class BaseBoxHistoriElement : public QHBoxLayout {
	public:
		explicit inline BaseBoxHistoriElement( \
			const char * expression, Window *window, \
			const char * result
		) noexcept : QHBoxLayout() {
			setSpacing(0);
			setContentsMargins(0, 0, 0, 0);
			addWidget(new LabelHistori{expression, "keyboard", window});
			char * commonCallback {new char[ \
				strlen(expression)+strlen(result)+2UL]{'\0'}};
			strcpy(commonCallback, expression);
			strcat(commonCallback, "=");
			strcat(commonCallback, result);
			addWidget(new LabelHistori{new char[]{"="}, "keyboard", \
				window, commonCallback});
			addWidget(new LabelHistori{result, "keyboard", window});
		}
	};
	class BasicBoxHistoriElement : public QVBoxLayout {
	public:
		explicit inline BasicBoxHistoriElement( \
			const char * expression, Window * window, \
			const char * result, const char * nameOperation \
		) noexcept : QVBoxLayout() {
			setSpacing(0);
			setContentsMargins(0, 0, 0, 0);
			addWidget(new LabelHistori{nameOperation, \
				"keyboard", window});
			addLayout(new BaseBoxHistoriElement{ \
				expression, window, result});
		}
	};
	class SubCustomBoxHistoriElement : public QHBoxLayout {
	public:
		explicit inline SubCustomBoxHistoriElement( \
			Window * window, \
			const char * label1, const char * text1, \
			const char * label2, const char * text2 \
		) noexcept : QHBoxLayout() {
			setSpacing(0);
			setContentsMargins(0, 0, 0, 0);
			addWidget(new LabelHistori{ \
				label1, "keybord", window});
			char *customCallback{new char[ \
				strlen(label1) + strlen(text1) + 2UL]{'\0'}};
			strcpy(customCallback, label1);
			strcat(customCallback, "=");
			strcat(customCallback, text1);
			addWidget(new LabelHistori{ \
				new char[]{"="}, "keybord", \
				window, customCallback});
			addWidget(new LabelHistori{ \
				text1, "keyboard", window});

			addWidget(new LabelHistori{ \
				label2, "keyboard", window});
			customCallback = new char[ \
				strlen(label1) + strlen(text1) + 2UL]{'\0'};
			strcpy(customCallback, label2);
			strcat(customCallback, "=");
			strcat(customCallback, text2);
			addWidget(new LabelHistori{ \
				new char[]{"="}, "keyboard", \
				window, customCallback});
			addWidget(new LabelHistori{ \
				text2, "keyboard", window});
		}
	};
	class CustomBoxHistoriElement : public QVBoxLayout {
	public:
		explicit inline CustomBoxHistoriElement( \
			const char * const expression, \
			Window * const  window, byte tab, \
			const char * const label1, \
			const char * const text1, \
			const char * const label2, \
			const char * const text2, \
			const char * const  nameOperation = nullptr \
		) noexcept : QVBoxLayout() {
			setSpacing(0);
			setContentsMargins(0, 0, 0, 0);
			if (nameOperation)
				addWidget(new LabelHistori{ \
					nameOperation, "keyboard", window});
			addLayout(new SubCustomBoxHistoriElement{ \
				window, label1, text1, \
				label2, text2});
			LineEdit * const lineEdit \
				{window->getLineEdit(tab, byte(2))};
			addLayout(new BaseBoxHistoriElement{ \
				lineEdit->text().toUtf8().data(), \
				window, window->getResult(tab, byte(2))});
		}
	};
}
namespace CreateHistori {
	class HistoriVBox : public QVBoxLayout {
	public:
		explicit HistoriVBox( \
			const char * label, Window *window \
		) : QVBoxLayout{} {
			setSpacing(0);
			setContentsMargins(0, 0, 0, 0);
			setObjectName("histori");
			addWidget(new NewHistoriElement::LabelHistori{ \
				label, "basic", window});
		}
	};

	class HistoriWidget : public QWidget {
	private:
		HistoriVBox *_addHistori = nullptr;
	public:
		explicit HistoriWidget( \
			const char * label, Window * window \
		) : QWidget() {
			setObjectName("histori");
			setLayout(_addHistori = new HistoriVBox{label, window});
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
		explicit HistoriScroll( \
			const char * label, Window * window \
		) : QScrollArea() {
			setHorizontalScrollBarPolicy( \
					Qt::ScrollBarPolicy::ScrollBarAlwaysOff \
			);
			setObjectName("histori");
			setWidgetResizable(true);
			setWidget(_resizeHistori = \
				new HistoriWidget{label, window});
		}
		HistoriWidget *getResizeHistori() {
			return _resizeHistori;
		}
	};
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
		char* pos = {strstr(_lineEditText, "_ALL")};
		memmove(pos, pos + 4, strlen(pos + 4) + 1); // Сдвигаем остаток строки влево

		if (strlen(_lineEditText) > 0) {
			// window->setResult(_lineEditText);  // Устанавливаем результат
			
			buttonOther();                         // Вызываем другую функцию
			addHistori();    // Добавляем в историю

			_window->setResult("0");            // Сбрасываем результат
		}

		_window->getLineEdit()->setText(""); // Очищаем поле ввода
	}

	void button_DO() {
		char* pos {strstr(_lineEditText, "_DO")};
		memmove(pos, pos + 3, strlen(pos + 3) + 1UL);

		if (strlen(_lineEditText) > 0)
			buttonOther(), addHistori();
		memmove(_lineEditText, pos, strlen(pos) + 1UL);
		_window->getLineEdit()->setText(_lineEditText);
	}

	void button_POS() {
		char* pos {strstr(_lineEditText, "_POS")};
		memmove(pos, pos + 4, strlen(pos + 4) + 1); // Сдвигаем остаток строки влево

		if (strlen(_lineEditText) > 0)
			buttonOther(), addHistori();
		*pos = '\0';
		_window->getLineEdit()->setText(_lineEditText);
	}

	void button_RES() {

		std::cout << _window << std::endl;
        const char *result = _window->getResult();
		char *pos = strstr(_lineEditText, "_RES");
		memmove(pos, pos + 4, strlen(pos + 4) + 1); // Сдвигаем остаток строки влево

		if (strlen(_lineEditText) > 0) {
            // window.setResult(_lineEditText);
            buttonOther();
            addHistori();
		}
        QLineEdit *line_edit {_window->getLineEdit()};
        line_edit->setText(result);
        line_edit->setCursorPosition(strlen(result));
	}

	void addHistori() {

		QLayout *elementGlobal {nullptr}, \
			*elementLocal {nullptr};
		byte tab = _window->getInputtin()[0];

		switch (tab) {
			case 3:
				elementGlobal = new NewHistoriElement:: \
					CustomBoxHistoriElement{ \
					_lineEditText, _window, 1, \
					"a", _window->getResult(1, 0), \
					"b", _window->getResult(1, 1), \
					"Integral"
				};
				elementLocal = new NewHistoriElement:: \
					CustomBoxHistoriElement{ \
					_lineEditText, _window, 1, \
					"a", _window->getResult(1, 0), \
					"b", _window->getResult(1, 1) \
				};
				break;

			case 4:
				elementGlobal = new NewHistoriElement:: \
					CustomBoxHistoriElement{ \
					_lineEditText, _window, 4, \
					"with", _window->getResult(3, 0), \
					"on", _window->getResult(4, 1), \
					"Replacement" \
				};
				elementLocal = new NewHistoriElement:: \
					CustomBoxHistoriElement{ \
					_lineEditText, _window, 4, \
					"with", _window->getResult(3, 0), \
					"on", _window->getResult(4, 1) \
				};
				break;

			default:
				{

					elementGlobal = new NewHistoriElement:: \
						BasicBoxHistoriElement{ \
						_lineEditText, _window, \
						_window->getResult(), lstTabs[tab] \
					};
					elementLocal = new NewHistoriElement:: \
						BaseBoxHistoriElement{ \
						_lineEditText, _window, \
						_window->getResult() \
					};
				}
				break;
		}
		std::cout << "Global" << elementGlobal << std::endl;
		std::cout << "Local" << elementLocal << std::endl;

		// Добавление элемента в глобальную историю
		_window->getAddGlobalHistori()->addLayout(elementGlobal);
		_window->getResizeGlobalHistori()->adjustSize();
		QScrollArea *globalHistori \
			{_window->getGlobalHistori()};
		globalHistori->verticalScrollBar()->setValue(
			globalHistori->verticalScrollBar()->maximum()
		);
		std::cout << "ok" << std::endl;

		// Добавление элемента в локальную историю
		_window->getAddLocalHistori(tab)->addLayout(elementLocal);
		_window->getResizeLocalHistori()->adjustSize();
		QScrollArea *localHistori \
			{_window->getLocalHistori()};
		localHistori->verticalScrollBar()->setValue(
			localHistori->verticalScrollBar()->maximum()
		);
	}

    void button_O() {
		char *pos = strstr(_lineEditText, "_O");
		if (pos == _lineEditText) *_lineEditText = '\0';
		else memmove(pos-1, pos + 2, strlen(pos + 2) + 1);
		_window->getLineEdit()->setText(_lineEditText);
	}

	
	void buttonOther() {
		puts("buttonOther");
		const byte * const inputtin {_window->getInputtin()};
		Check::Error * error {Check:: \
			validateExpression(_lineEditText)};
		if (error) {
			char *errStr{new char[30]{'\0'}};
			sprintf(errStr, "%lu %s '%c'", \
				error->errorPos, error->message, \
				error->errorChar);
			_window->setWindowTitle(errStr);
			delete [] errStr;
			delete error;
			return;
		}
		const char * result {nullptr};
		Expression * const expression {Expression:: \
			buildExpressionTree(_lineEditText, nullptr, false) \
		};
		if (!expression) return;
		switch (*inputtin) {
			case 0:
				result = expression->calc();
				break;
			case 1:
				result = expression-> \
					differentiate()->print();
				break;
			case 2:
				result = expression-> \
					integrate()->print();
				break;
			default:
				break;
		}
		_window->setResult(result);
		_window->setWindowTitle(windowTitle);
		delete expression;
		//_window->setForResult(window->result);
	}
            
	static void inputtinLineEdit( \
		QPushButton *button, Window *window \
	) {
		const char *label = strdup(button->text().toUtf8().constData());
		LineEdit *lineEdit \
			{window->getLineEdit()};
		const char *text {lineEdit->text().toUtf8().data()};
		size_t positionCursor = lineEdit->cursorPosition();
		char *result {new char[strlen(text)+strlen(label)+1UL]{""}};
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
void LineEdit::onLineEditChanged( \
		const QString& text \
) const noexcept {
	// Преобразуем QString в QByteArray
	QByteArray byteArray = text.toUtf8();

	const qsizetype len { byteArray.size() };
	if (!len) { _window->setResult(strdup("0")); return; }
	// Копируем данные QByteArray в char[]
	char *textCh {new char[len + 1UL]{""}}; // +1 для нулевого завершающего символа
	strcpy(textCh, byteArray.constData());
	LogicCalculate *logicCalculate \
		{new LogicCalculate(textCh, _window)};
	if (strstr(textCh, "_ALL") != nullptr)
		logicCalculate->button_ALL();
	else if (strstr(textCh, "_O") != nullptr)
		logicCalculate->button_O();
	else if (strstr(textCh, "_RES") != nullptr)
		logicCalculate->button_RES();
	else if (strstr(textCh, "_DO"))
		logicCalculate->button_DO();
	else if (strstr(textCh, "_POS"))
		logicCalculate->button_POS();
	else
		logicCalculate->buttonOther();
	delete [] textCh;
}

namespace Title {
	class TitleLayout : public QHBoxLayout {
	private:
		const Window                 *_window                  {nullptr};
		CreateHistori::HistoriScroll *_globalHistori           {nullptr};
		CreateHistori::HistoriScroll *_localHistoriBasic       {nullptr};
		CreateHistori::HistoriScroll *_localHistoriIntegral    {nullptr};
		CreateHistori::HistoriScroll *_localHistoriDerivative  {nullptr};
		CreateHistori::HistoriScroll *_localHistoriIntegrate   {nullptr};
		CreateHistori::HistoriScroll *_localHistoriReplacement {nullptr};
		Button::ButtonBase           *_buttonChangeHistori     {nullptr};
	public:
		explicit TitleLayout( \
				CalculateDragAndDrop *app, Window *window \
		) noexcept : _window(window), QHBoxLayout() {
			setContentsMargins(10, 10, 10, 10);
        	setSpacing(10);
			puts("op");

			addWidget( \
				new Button::ButtonBase{ \
					"Settings", window, nullptr, "basic" \
				}
			);
			addWidget( \
				_buttonChangeHistori = new Button::ButtonBase{ \
					"Global histori", window, \
					new std::function<void(QPushButton *)> { \
						std::bind(
							&TitleLayout::changeHistoriVisible, \
							this, std::placeholders::_1 \
						) \
					}, "basic" \
				} \
			);
			addWidget( \
				new Button::ButtonBase{ \
					"+ New", window, \
					new std::function<void(QPushButton *)> { \
						std::bind( \
							&CalculateDragAndDrop::createWindow, app, std::placeholders::_1 \
						) \
					}, "basic" \
				} \
			);
			return;
		}
		void buttonInit(void) {
			const Window * window = _window;
			_globalHistori           = window->getGlobalHistori();
			_localHistoriBasic       = window->getLocalHistori(0);
			_localHistoriIntegral    = window->getLocalHistori(1);
			_localHistoriDerivative  = window->getLocalHistori(2);
			_localHistoriIntegrate   = window->getLocalHistori(3);
			_localHistoriReplacement = window->getLocalHistori(4);
			//std::cout << _globalHistori << std::endl << _localHistoriBasic << std::endl << \
				_localHistoriIntegral << std::endl << _localHistoriDerivative << std::endl << \
				_localHistoriIntegrate << std::endl << _localHistoriReplacement << std::endl;
			return;
		}
		void changeHistoriVisible(QPushButton *button) const {
			if (_globalHistori->isVisible()) {
				_buttonChangeHistori->setText("Global Histori");
				_globalHistori->setVisible(false);
				_localHistoriBasic->setVisible(true);
				_localHistoriIntegral->setVisible(true);
				_localHistoriDerivative->setVisible(true);
				_localHistoriIntegrate->setVisible(true);
				_localHistoriReplacement->setVisible(true);
			} else {
				_buttonChangeHistori->setText("Local Histori");
				_globalHistori->setVisible(true);
				_localHistoriBasic->setVisible(false);
				_localHistoriIntegral->setVisible(false);
				_localHistoriDerivative->setVisible(false);
				_localHistoriIntegrate->setVisible(false);
				_localHistoriReplacement->setVisible(false);
			}
			return;
		}
	};
	class TitleBar : public QWidget {
	private:
		TitleLayout *_child {nullptr};
	public:
		explicit TitleBar( \
			CalculateDragAndDrop *app, Window *window \
		) noexcept : QWidget() {
			setLayout( \
				_child = new TitleLayout(app, window) \
			);
		}
		TitleLayout *getChild(void) noexcept {
			return _child;
		}
	};
}



namespace Grid {
	template<class TButton, byte rowSize, byte columnSize>
	void buildingGridKeyboard( \
		const char * const (&buttons)[rowSize][columnSize], \
		QGridLayout *grid, Window *window, \
		const byte rowStart, const byte columnStart, \
		const char * cssName \
	) noexcept {
		std::function<void(QPushButton *)> * func { \
			new std::function<void(QPushButton*)>{
				[window](QPushButton* btn) {
					LogicCalculate::inputtinLineEdit(btn, window);
					return;
				} \
			}
		};

		byte rowEnd {(byte)(rowSize + rowStart)}, \
			columnEnd {(byte)(columnSize + columnStart)};
		byte row {rowStart};
		for (const char * const * ptr { \
			reinterpret_cast<const char * const *>( \
			buttons)}; row != rowEnd; row++ \
		) {
			for (byte column{columnStart}; \
				column != columnEnd; column++, ptr++ \
			) {
				grid->addWidget(new TButton { \
					*ptr, window, func, cssName\
				}, row, column, 1, 1);
			}
		}
	}
	template<class TButton, byte rowSize, byte columnSize>
	class GridCalculateKeyboard : public QGridLayout {
	public:
		explicit GridCalculateKeyboard( \
			const char * const (&buttons)[rowSize][columnSize], \
			Window *window, const char * cssName \
		) noexcept : QGridLayout() {
			setContentsMargins(10, 10, 10, 10);
			setSpacing(10);
			buildingGridKeyboard<TButton, rowSize, columnSize>( \
				buttons, this, window, byte(0), \
				byte(0), cssName \
			);
		}
	};


	class GridCalculateCommon : public QGridLayout {
	private:
	public:
		explicit GridCalculateCommon( \
			Window *window \
		) : QGridLayout{} {
			setSpacing(10);
			setContentsMargins(10, 10, 10, 10);
			std::cout << "po" << window << std::endl;

			buildingGridKeyboard<Button::ButtonDrag, \
				rowFunctions, columnFunctions>(
				functions, this, window, \
				byte(0), byte(0), "function" \
			);

			buildingGridKeyboard<Button::ButtonDragAndDrop, \
				rowBracketsMain, columnBracketsMain>(
				bracketsMain, this, window, \
				byte(1), byte(0), "bracket" \
			);

			buildingGridKeyboard<Button::ButtonDragAndDrop, \
				rowNumbersMain, columnNumbersMain>( \
				numbersMain, this, window, \
				byte(2), byte(0), "number" \
			);

			buildingGridKeyboard<Button::ButtonDragAndDrop, \
				rowOperatorsMain, columnOperatorsMain>(
				operatorsMain, this, window, \
				byte(1), byte(3), "operator" \
			);

			buildingGridKeyboard<Button::ButtonDragAndDrop, \
				rowConstsMain, columnConstsMain>(
				constsMain, this, window, \
				byte(5), byte(3), "const" \
			);

			buildingGridKeyboard<Button::ButtonDragAndDrop, \
				rowEmptyMain, columnEmptyMain>(
				emptyMain, this, window, \
				byte(6), byte(0), "empty" \
			);

			Button::ButtonDrag *resultButton {\
				new Button::ButtonDrag { \
					window->getResult(0, 0), \
					window, nullptr, "number" \
				}
			};
			window->setResultButton(resultButton);
			addWidget(resultButton, 7, 0, 1, 5);
		}
	};

	class GridBaseCalc : public QGridLayout {
	public:
		explicit GridBaseCalc( \
			Window *window, byte tab \
		) : QGridLayout() {
			setSpacing(12);
			setContentsMargins(10, 0, 10, 0);
			CreateHistori::HistoriScroll *localHistori \
				{new CreateHistori::HistoriScroll{ \
					"Local histori", window}};
			window->setLocalHistori(localHistori, tab);
			CreateHistori::HistoriWidget *resizeLocalHistori \
				{localHistori->getResizeHistori()};
			window->setResizeLocalHistori(resizeLocalHistori, tab);
			window->setAddLocalHistori(resizeLocalHistori->getAddHistori(), tab);
			addWidget(localHistori, 0, 0, 1, 6);
		}
	};
	class GridDefaultCalc : public GridBaseCalc {
	public:
		explicit GridDefaultCalc( \
			Window *window, byte tab \
		) : GridBaseCalc(window, tab) {
			LineEdit *lineEdit \
				{new LineEdit{window, tab, byte(0)}};
			window->setLineEdit(lineEdit, tab, byte(0));
			addWidget(lineEdit, 1, 0, 1, 6);
		}
	};
	class GridIntegralCalc : public GridBaseCalc {
	public:
		explicit GridIntegralCalc( \
			Window *window, byte tab = byte(3) \
		) : GridBaseCalc(window, tab) {
			addWidget( \
				new Button::ButtonBase{ \
					"a = ", window, nullptr \
				}, 1, 0, 1, 1 \
			);
			LineEdit *aLineEdit \
				{new LineEdit{window, 1, 0, "1"}};
			window->setLineEdit(aLineEdit, tab, byte(0));
			addWidget(aLineEdit, 1, 1, 1, 2);
			addWidget( \
				new Button::ButtonBase{ \
					"b = ", window, nullptr \
				}, 1, 3, 1, 1 \
			);
			LineEdit *bLineEdit \
				{new LineEdit{window, tab, byte(1), "2"}};
			window->setLineEdit(bLineEdit, tab, byte(1));
			addWidget(bLineEdit, 1, 4, 1, 2);
			LineEdit *mainLineEdit \
				{new LineEdit{window, tab, byte(2)}};
			window->setLineEdit(mainLineEdit, tab, byte(2));
			addWidget(mainLineEdit, 2, 0, 1, 6);
		}
	};
	class GridReplacementCalc : public GridBaseCalc {
	public:
		explicit GridReplacementCalc( \
			Window *window, byte tab = 4 \
		) : GridBaseCalc{window, tab} {
			addWidget( \
				new Button::ButtonBase{ \
					"with =", window, \
					nullptr \
				}, 1, 0, 1, 1 \
			);
			LineEdit *withLineEdit = \
				new LineEdit(window, tab, byte(0), "x");
			window->setLineEdit(withLineEdit, tab, byte(0));
			addWidget(withLineEdit, 1, 1, 1, 2);
			addWidget( \
				new Button::ButtonBase( \
					"on =", window, \
					nullptr \
				), 1, 3, 1, 1 \
			);
			LineEdit *onLineEdit \
				{new LineEdit{window, tab, byte(1), "0"}};
			window->setLineEdit(onLineEdit, tab, byte(1));
			addWidget(onLineEdit, 1, 4, 1, 2);
			LineEdit *mainLineEdit \
				{new LineEdit{window, tab, byte(2)}};
			window->setLineEdit(mainLineEdit, tab, byte(2));
			addWidget(mainLineEdit, 2, 0, 1, 6);
		}
	};
}



namespace TabWindow {
	class TabQWidget : public QWidget {
	public:
		explicit TabQWidget( \
			QGridLayout *tab \
		) : QWidget() {
			setLayout(tab);
		}
	};


	class TabWidgetKeyboard : public QTabWidget {
	public:
		explicit TabWidgetKeyboard( \
			Window *window \
		) : QTabWidget() {
			addTab(new TabQWidget{ \
				new Grid::GridCalculateKeyboard< \
					Button::ButtonDrag, rowDigits10, \
					columnDigits10>{ \
					digits10, window, "number" \
				} \
			}, "digits 10");
			addTab(new TabQWidget{ \
				new Grid::GridCalculateKeyboard< \
					Button::ButtonDrag, rowDigits16, \
					columnDigits16>{ \
					digits16, window, "number" \
				} \
			}, "digits 16");
			addTab(new TabQWidget{ \
				new Grid::GridCalculateKeyboard< \
					Button::ButtonDrag, rowOperators1, \
					columnOperators1>{ \
					operators1, window, "operator" \
				} \
			}, "operators 1");
			addTab(new TabQWidget{ \
				new Grid::GridCalculateKeyboard< \
					Button::ButtonDrag, rowOperators2, \
					columnOperators2>{ \
					operators2, window, "operator" \
				} \
			}, "operators 2");
			addTab(new TabQWidget{ \
				new Grid::GridCalculateKeyboard< \
					Button::ButtonDrag, rowBrackets, \
					columnBrackets>{ \
					brackets, window, "bracket" \
				} \
			}, "brackets");
			addTab(new TabQWidget{ \
				new Grid::GridCalculateKeyboard< \
					Button::ButtonDrag, rowBraces, columnBraces>{ \
					braces, window, "bracket" \
				} \
			}, "braces");
			addTab(new TabQWidget{ \
				new Grid::GridCalculateKeyboard< \
					Button::ButtonDrag, rowConsts, columnConsts>{ \
					consts, window, "const" \
				} \
			}, "consts");
			addTab(new TabQWidget{ \
				new Grid::GridCalculateKeyboard< \
					Button::ButtonDrag, rowTrigonometricFuncs, \
					columnTrigonometricFuncs>{ \
					trigonometricFuncs, window, "operator" \
				} \
			}, "trigonometric functions");
			addTab(new TabQWidget{ \
				new Grid::GridCalculateKeyboard< \
					Button::ButtonDrag, \
					rowInverseTrigonometricFuncs, \
					columnInverseTrigonometricFuncs>{
					inverseTrigonometricFuncs, \
					window, "operator" \
				} \
			}, "inverse trigonometric functions");
			addTab(new TabQWidget{ \
				new Grid::GridCalculateKeyboard< \
					Button::ButtonDrag, rowPrefixes, \
					columnPrefixes>{ \
					prefixes, window, "prefix" \
				} \
			}, "prefix");
			addTab(new TabQWidget{ \
				new Grid::GridCalculateKeyboard< \
					Button::ButtonDrag, rowFunctions, \
					columnFunctions>{ \
					functions, window, "function" \
				} \
			}, "functions");
			tabBar()->setElideMode(Qt::ElideNone);
		}
	};



	//Main TabWidget

	class MainTabWidget : public QTabWidget {
	public:
		explicit MainTabWidget( \
			Window *window \
		) : QTabWidget() {
			addTab(new TabQWidget{ \
				new Grid::GridDefaultCalc{window, byte(0)} \
			}, lstTabs[0]);
			addTab(new TabQWidget{ \
				new Grid:: \
					GridDefaultCalc{window, byte(1)} \
			}, lstTabs[1]);
			addTab(new TabQWidget{ \
				new Grid:: \
					GridDefaultCalc{window, byte(2)} \
			}, lstTabs[2]);
			addTab(new TabQWidget{ \
				new Grid::GridIntegralCalc(window) \
			}, lstTabs[3]);
			addTab(new TabQWidget{ \
				new Grid::GridReplacementCalc(window) \
			}, lstTabs[4]);
			tabBar()->setElideMode(Qt::ElideNone);
		}
	};
}


class MainLayout : public QVBoxLayout {
public:
	explicit MainLayout( \
		CalculateDragAndDrop *app, Window *window \
	) : QVBoxLayout() {
		std::cout << window << std::endl;
		setContentsMargins(0, 0, 0, 0);
		setSpacing(0);
		Title::TitleBar * const titleBar \
			{new Title::TitleBar{app, window}};
		addWidget(titleBar);
		CreateHistori::HistoriScroll *globalHistori \
			{new CreateHistori::HistoriScroll{ \
				"Gloabl histori", window}};
		globalHistori->setVisible(false);
		window->setGlobalHistori(globalHistori);
		CreateHistori::HistoriWidget *resizeGlobalHistori \
			{globalHistori->getResizeHistori()};
		window->setResizeGlobalHistori(resizeGlobalHistori);
		window->setAddGlobalHistori( \
			resizeGlobalHistori->getAddHistori());
		addWidget(globalHistori);
		addWidget(new TabWindow::MainTabWidget{window});
		addLayout(new Grid::GridCalculateCommon{window});
		TabWindow::TabWidgetKeyboard * keyboard { \
			new TabWindow::TabWidgetKeyboard{window}
		};
		addWidget(keyboard);
		std::function<void(QPushButton*)> * func { \
			new std::function<void(QPushButton *)> { \
				[keyboard](QPushButton * button) -> void {
					bool isHide {keyboard->isVisible()};
					if (isHide)
						button->setText("open");
					else button->setText("close");
					keyboard->setVisible(!isHide);
				}
			}
		};
		addWidget(new Button::ButtonBase{ \
			"close", window, func, "hide"});
		titleBar->getChild()->buttonInit();
		setStretch(0, 1);
		setStretch(1, 20);
		setStretch(2, 20);
		setStretch(3, 50);
		setStretch(4, 1);
		setStretch(5, 1);
		return;
	}
};

class MainWidget : public QWidget {
public:
	explicit inline MainWidget( \
		CalculateDragAndDrop *app, Window *window \
	) noexcept : QWidget(window) {
		setLayout(new MainLayout{app, window});
	}
};

inline void Window::postInit() noexcept {
    setCentralWidget(new MainWidget{_app, this});
	return;
}

inline Window::~Window(void) {
	LineEdit *** ptrLineEdit \
		{const_cast<LineEdit ***>(_lineEdit)};
	const char ***ptrResult {const_cast<const char ***>(_result)};
	deleteResultOrLineEdit(ptrLineEdit);
	deleteResultOrLineEdit(ptrResult);
	size_t len {COUNT_LOCAL_HISTORI};
	CreateHistori::HistoriScroll ** ptrLocalHistori{_localHistori};
	CreateHistori::HistoriWidget ** ptrResizeLocalHistori{_resizeLocalHistori};
	CreateHistori::HistoriVBox ** ptrAddLocalHistori{_addLocalHistori};
	for (; len--; ptrLocalHistori++, \
		ptrResizeLocalHistori++, \
		ptrAddLocalHistori++ \
	) {
		delete *ptrLocalHistori;
		delete *ptrResizeLocalHistori;
		delete *ptrAddLocalHistori;
		delete [] *ptrLineEdit;
	}
	delete [] _localHistori;
	delete [] _resizeLocalHistori;
	delete [] _addLocalHistori;

	delete [] _lineEdit;
	delete [] _inputtin;
	delete _resultButton;
	delete _globalHistori;
	delete _addGlobalHistori;
	delete _resizeGlobalHistori;
	QWidget::~QWidget();
	return;
}