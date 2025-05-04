#include <vector>
#include <tuple>
#include <functional>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <memory>
#include <QPushButton>
#include <QApplication>
#include <QMainWindow>
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
#include <QLabel>

#define BYTE_MAX 255
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
	class Action;
	class Menu;
	class TitleLayout;
	class TitleBar;
}
namespace Grid {
	template<class TButton>
	class BuildingGridKeyboard;
	template<class TButton>
	class GridCalculateKeyboard;
	class GridCalculateCommon;
	class GridBaseCalc;
	class GridBasicCalc;
	class GridIntegralCalc;
	class GridDerivateOrIntegrateCalc;
	class GridReplacementCalc;
}
namespace TabWindow {
	class TabQWidget;
	class TabWidgetKeyboard;
	class MainTabWidget;
}
class MainLayout;

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
				background-color: rgb(118, 176, 48);
			}
			#operator {
				background-color: rgb(187, 180, 52);
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
			QLineEdit {
				background: transparent;
				border: 1px solid white;
				text-align: center;
				color: white;
			}
			QPushButton {
				color: white;
				border-radius: 10px;
				border: 1px solid white;
				font-size: 20px;
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
			
			QTabBar::tab {
				padding: 8px 12px;
				margin: 5px;
				border: 1px solid white;
				border-radius: 10px;
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
	) : QMainWindow(), _app{app} {
		setWindowTitle("CalculateDragAndDrop");
		resize(400, 800);
		setObjectName("Window");
		show();
		return;
	}
	/*
	inline void paintEvent(QPaintEvent *event) noexcept override {
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
	*/

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
		CreateHistori::HistoriScroll* newLocalHistori, \
		byte index \
	) noexcept {
		_localHistori[index] = newLocalHistori;
		return;
	}	
	inline void setLocalHistori( \
		CreateHistori::HistoriScroll* newLocalHistori \
	) noexcept {
		_localHistori[_inputtin[0]] = newLocalHistori;
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
		return;
	}
	inline void setResult( \
		const char *newResult \
	) noexcept {
		const char * & res {_result[*_inputtin][_inputtin[1]]};
		delete [] res;
		res = newResult;
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
			const char *cssName = "keybord", QMenu *menu = nullptr \
		) : _window(window), QPushButton(label) {
			setContentsMargins(0, 0, 0, 0);
			if (callback) 
				connect(this, &QPushButton::clicked, [this, callback](bool) {
					if (callback) {
						(*callback)(this);
					}
				});
			if (menu)
				setMenu(menu);
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
			const char *cssName = "keyboard", QMenu *menu = nullptr \
		) : ButtonBase(
			label, window, callback, cssName, menu
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
			const char *label, Window *window, \
			std::function<void(QPushButton *)> *callback = nullptr, \
			const char *cssName = "keyboard", QMenu *menu = nullptr \
		) : ButtonDrag ( \
			label, window, callback, cssName, menu \
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
class  LineEdit : public QLineEdit {
private:
	Window *_window;
	byte tab {0}, index {0};
	void onLineEditChanged( \
		const QString& text \
	) const noexcept;
public:
	explicit LineEdit ( \
		Window *window, byte tab, byte index, \
		const char *text = "" \
	) : _window(window), QLineEdit() {
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
			_window->setInputtin(tab, index);
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
			Window * const  window, \
			byte tab, const char * const  nameOperation, \
			const char * const label1, \
			const char * const text1, \
			const char * const label2, \
			const char * const text2 \
		) noexcept : QVBoxLayout() {
			setSpacing(0);
			setContentsMargins(0, 0, 0, 0);
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

		_window->getLineEdit()->setText(""); // Очищаем поле ввода
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
		_window->getLineEdit()->setText(_lineEditText);
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
		_window->getLineEdit()->setText(pos);
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
        QLineEdit *line_edit {_window->getLineEdit()};
        line_edit->setText(result);
        line_edit->setCursorPosition(strlen(result)-1);
	}

	void addHistori() {

		QLayout *element{nullptr};
		short tab = _window->getInputtin()[0];

		switch (tab) {
			case 1:
				element = new NewHistoriElement:: \
					CustomBoxHistoriElement{ \
					_lineEditText, _window, \
					1, "Integral", \
					"a", _window->getResult(1, 0), \
					"b", _window->getResult(1, 1) \
				};
				break;

			case 4:
				element = new NewHistoriElement:: \
					CustomBoxHistoriElement{ \
					_lineEditText, _window, \
					4, "Replacement", \
					"with", _window->getResult(3, 0), \
					"on", _window->getResult(4, 1) \
				};
				break;

			default:
				{
					std::map<short, const char*> lstTabs = {
						{0, "Basic"}, {2, "Derivate"}, {3, "Integrate"}
					};
					element = new NewHistoriElement:: \
						BasicBoxHistoriElement{ \
						_lineEditText, _window, \
						_window->getResult(), lstTabs[tab] \
					};
				}
				break;
		}


		// Добавление элемента в глобальную историю
		_window->getAddGlobalHistori()->addLayout(element);
		_window->getResizeGlobalHistori()->adjustSize();
		QScrollArea *globalHistori \
			{_window->getGlobalHistori()};
		globalHistori->verticalScrollBar()->setValue(
			globalHistori->verticalScrollBar()->maximum()
		);

		// Добавление элемента в локальную историю
		_window->getAddLocalHistori()->addLayout(element);
		_window->getResizeLocalHistori()->adjustSize();
		QScrollArea *localHistori \
			{_window->getLocalHistori()};
		localHistori->verticalScrollBar()->setValue(
			localHistori->verticalScrollBar()->maximum()
		);
	}

    void button_O() {
		char *pos = strstr(_lineEditText, "_O");
		memmove(pos-1, pos + 3, strlen(pos + 3) + 1);

		_window->getLineEdit()->setText(_lineEditText);
	}

	
	void buttonOther() {
		auto integral = [_window = this->_window]() {
			char* equation = strdup( \
				_window->getLineEdit(1, 2)->text().toUtf8().data() \
			);
			// _window->setResult(equation, 1, 2);
		};

		auto otherTab = [_window = this->_window]() {
			// _window->setResult(_window->getLineEdit(), 0, 0);
		};
		const byte * inputtin {_window->getInputtin()};
		switch (*inputtin * 10 + inputtin[1]) {
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

		//_window->setForResult(window->result);
	}
            
	static void inputtinLineEdit( \
		QPushButton *button, Window *window \
	) {
		const char *label = strdup(button->text().toUtf8().data());
		QLineEdit *lineEdit \
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

	// Копируем данные QByteArray в char[]
	char textCh[byteArray.size() + 1]; // +1 для нулевого завершающего символа
	strcpy(textCh, byteArray.constData());
	LogicCalculate *logicCalculate \
		{new LogicCalculate(textCh, _window)};
	if (strstr(textCh, "_ALL") != nullptr)
		logicCalculate->button_ALL();
	else if (strstr(textCh, "_O") != nullptr)
		logicCalculate->button_O();
	else if (strstr(textCh, "_RES") != nullptr)
		logicCalculate->button_RES();
	else
		logicCalculate->buttonOther();
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
				std::vector<Button::ButtonBase *> buttons \
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
			setContentsMargins(0, 0, 0, 0);
        	setSpacing(0);
			puts("op");

			addWidget( \
				new Button::ButtonBase{ \
					"Settings", window \
				}
			);
			addWidget( \
				_buttonChangeHistori = new Button::ButtonBase{ \
					"Global Histori", window, \
					new std::function<void(QPushButton *)> { \
						std::bind(
							&TitleLayout::changeHistoriVisible, \
							this, std::placeholders::_1 \
						) \
					} \
				} \
			);
			addWidget( \
				new Button::ButtonBase{ \
					"+ Add", window, \
					new std::function<void(QPushButton *)>( \
						std::bind( \
							&CalculateDragAndDrop::createWindow, app, std::placeholders::_1 \
						) \
					) \
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
			std::cout << _globalHistori << std::endl << _localHistoriBasic << std::endl << \
				_localHistoriIntegral << std::endl << _localHistoriDerivative << std::endl << \
				_localHistoriIntegrate << std::endl << _localHistoriReplacement << std::endl;
			return;
		}
		void changeHistoriVisible(QPushButton *button) const {
			if (_globalHistori->isVisible()) {
				_buttonChangeHistori->setText("Local Histori");
				_globalHistori->setVisible(false);
				_localHistoriBasic->setVisible(true);
				_localHistoriIntegral->setVisible(true);
				_localHistoriDerivative->setVisible(true);
				_localHistoriIntegrate->setVisible(true);
				_localHistoriReplacement->setVisible(true);
			} else {
				_buttonChangeHistori->setText("Global Histori");
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
			setFixedHeight(35);
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
	template<class TButton>
	class BuildingGridKeyboard {
	public:
		explicit inline BuildingGridKeyboard( \
			std::vector<std::vector<const char *>> buttons, \
			QGridLayout *grid, Window *window, \
			byte row = byte(0), byte columnStart = byte(0), \
			const char * cssName = "opertor" \
		) noexcept {
			static std::function<void(QPushButton *)> * func { \
				new std::function<void(QPushButton*)>{
					[window](QPushButton* btn) {
						LogicCalculate::inputtinLineEdit(btn, window);
						return;
					} \
				}
			};

			const std::vector<std::vector<const char *>>::const_iterator \
				itRowEnd {buttons.cend()};
			std::vector<const char *>::const_iterator itColumnEnd {}, \
				itColumn {};
			byte column {byte(0)};
			for ( \
				std::vector<std::vector<const char *>>::const_iterator \
				itRow {buttons.cbegin()}; itRow != itRowEnd; \
				itRow++, row++ \
			) {
				itColumnEnd = itRow->cend();
				for ( \
					itColumn = itRow->cbegin(), column = columnStart; \
					itColumn != itColumnEnd; itColumn++, column++ \
				) {
					const char * const labelButton = *itColumn;
					grid->addWidget(new TButton { \
						labelButton, window, func, cssName\
					}, row, column, 1, 1);
				}
			}
		}
	};
	template<class TButton>
	class GridCalculateKeyboard : public QGridLayout {
	public:
		explicit GridCalculateKeyboard( \
			std::vector<std::vector<const char *>> buttons, \
			Window *window, const char * cssName \
		) noexcept : QGridLayout() {
			setContentsMargins(10, 10, 10, 10);
			setSpacing(10);
			BuildingGridKeyboard<TButton>{ \
				buttons, this, window, byte(0), \
				byte(0), cssName \
			};
		}
	};


	class GridCalculateCommon : public QGridLayout {
	private:
		Window *_window {nullptr};
	public:
		explicit GridCalculateCommon( \
			Window *window \
		) : _window{window} {
			setSpacing(10);
			setContentsMargins(10, 10, 10, 10);

			BuildingGridKeyboard<Button::ButtonDrag>{
				std::vector<std::vector<const char *>> {
					{"_ALL", "_DO", "_RES", "_POS", "_O"}
				}, this, window, byte(0), byte(0), "function" \
			};

			BuildingGridKeyboard<Button::ButtonDragAndDrop>{
				std::vector<std::vector<const char *>> {
					{"()", "(", ")"},
				}, this, window, byte(1), byte(0), "bracket" \
			};

			BuildingGridKeyboard<Button::ButtonDragAndDrop>{ \
				std::vector<std::vector<const char *>> {
					{"7", "8", "9"},
					{"4", "5", "6"},
					{"1", "2", "3"},
					{"0", ".", "%"},
				}, this, window, byte(2), byte(0), "number" \
			};

			BuildingGridKeyboard<Button::ButtonDragAndDrop>{
				std::vector<std::vector<const char *>> {
					{"mod", "sqrt"},
					{"*", ":"},
					{"+", "-"},
					{"^", "!"},
				}, this, window, byte(1), byte(3), "operator"\
			};

			BuildingGridKeyboard<Button::ButtonDragAndDrop>{
				std::vector<std::vector<const char *>> {
					{"_PI", "_E"}
				}, this, window, byte(5), byte(3), "const"\
			};

			BuildingGridKeyboard<Button::ButtonDragAndDrop>{
				std::vector<std::vector<const char *>> {
					{"", "", "", "", ""}
				}, this, window, byte(6), byte(0), "empty"\
			};

			puts("hj");
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
			setContentsMargins(0, 0, 0, 0);
			CreateHistori::HistoriScroll *localHistori \
				{new CreateHistori::HistoriScroll{}};
			window->setLocalHistori(localHistori, tab);
			CreateHistori::HistoriWidget *resizeLocalHistori \
				{new CreateHistori::HistoriWidget{}};
			window->setResizeLocalHistori(resizeLocalHistori, tab);
			window->setAddLocalHistori(resizeLocalHistori->getAddHistori(), tab);
			addWidget(localHistori, 0, 0, 1, 6);
		}
	};

	class GridBasicCalc : public GridBaseCalc {
	public:
		explicit GridBasicCalc( \
			Window *window \
		) : GridBaseCalc(window, byte(0)) {
			LineEdit *lineEdit = \
				new LineEdit{window, byte(0), byte(0)};
			window->setLineEdit(lineEdit, byte(0), byte(0));
			addWidget(lineEdit, 1, 0, 1, 6);
		}
	};


	class GridIntegralCalc : public GridBaseCalc {
	public:
		explicit GridIntegralCalc( \
			Window *window \
		) : GridBaseCalc(window, byte(1)) {
			addWidget( \
				new Button::ButtonBase{ \
					"a = ", window, \
					nullptr, "calculate" \
				}, 1, 0, 1, 1 \
			);
			LineEdit *aLineEdit \
				{new LineEdit{window, 1, 0, "1"}};
			window->setLineEdit(aLineEdit, byte(1), byte(0));
			addWidget(aLineEdit, 1, 1, 1, 2);
			addWidget( \
				new Button::ButtonBase{ \
					"b = ", window, \
					nullptr, "calculate" \
				}, 1, 3, 1, 1 \
			);
			LineEdit *bLineEdit \
				{new LineEdit{window, byte(1), byte(1), "2"}};
			window->setLineEdit(bLineEdit, byte(1), byte(1));
			addWidget(bLineEdit, 1, 4, 1, 2);
			LineEdit *mainLineEdit \
				{new LineEdit{window, byte(1), byte(2)}};
			window->setLineEdit(mainLineEdit, byte(1), byte(2));
			addWidget(mainLineEdit, 2, 0, 1, 6);
		}
	};

	class GridDerivativeOrIntegrateCalc : public GridBaseCalc {
	public:
		explicit GridDerivativeOrIntegrateCalc( \
			Window *window, byte tab \
		) : GridBaseCalc(window, tab) {
			LineEdit *lineEdit \
				{new LineEdit{window, tab, byte(0)}};
			window->setLineEdit(lineEdit, tab, byte(0));
			addWidget(lineEdit, 1, 0, 1, 6);
		}
	};
	class GridReplacementCalc : public GridBaseCalc {
	public:
		explicit GridReplacementCalc( \
			Window *window \
		) : GridBaseCalc{window, byte(4)} {
			addWidget( \
				new Button::ButtonBase{ \
					"with =", window, \
					nullptr, "calculate" \
				}, 1, 0, 1, 1 \
			);
			LineEdit *withLineEdit = \
				new LineEdit(window, byte(4), byte(0), "x");
			window->setLineEdit(withLineEdit, byte(4), byte(0));
			addWidget(withLineEdit, 1, 1, 1, 2);
			addWidget( \
				new Button::ButtonBase( \
					"on =", window, \
					nullptr, "calculate" \
				), 1, 3, 1, 1 \
			);
			LineEdit *onLineEdit \
				{new LineEdit{window, byte(4), byte(1), "0"}};
			window->setLineEdit(onLineEdit, byte(4), byte(1));
			addWidget(onLineEdit, 1, 4, 1, 2);
			LineEdit *mainLineEdit \
				{new LineEdit{window, byte(4), byte(2)}};
			window->setLineEdit(mainLineEdit, byte(4), byte(2));
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
				new Grid::GridCalculateKeyboard<Button::ButtonDrag>{ \
					std::vector<std::vector<const char *>>{
						{"1", "2", "3", "4", "5"},
						{"6", "7", "8", "9", "0"}
					}, window, "number" \
				} \
			}, "digits 10");
			addTab(new TabQWidget{ \
				new Grid::GridCalculateKeyboard<Button::ButtonDrag>{ \
					std::vector<std::vector<const char *>>{
						{"A", "B", "C"},
						{"D", "E", "F"}
					}, window, "number" \
				} \
			}, "digits 16");
			addTab(new TabQWidget{ \
				new Grid::GridCalculateKeyboard<Button::ButtonDrag>{ \
					std::vector<std::vector<const char *>>{
						{"+", "-", ":", "*", "^"},
						{"!", "sqrt", "ln", "log", "lg"}
					}, window, "operator" \
				} \
			}, "operators");
			addTab(new TabQWidget{ \
				new Grid::GridCalculateKeyboard<Button::ButtonDrag>{ \
					std::vector<std::vector<const char *>>{
						{"()", ")", ")"}
					}, window, "bracket" \
				} \
			}, "brackets");
			addTab(new TabQWidget{ \
				new Grid::GridCalculateKeyboard<Button::ButtonDrag>{ \
					std::vector<std::vector<const char *>>{
						{"_E", "_PI"}
					}, window, "const" \
				} \
			}, "consts");
			addTab(new TabQWidget{ \
				new Grid::GridCalculateKeyboard<Button::ButtonDrag>{ \
					std::vector<std::vector<const char *>>{
						{"sin", "cos", "tan"},
						{"sec", "csc", "cot"}
					}, window, "operator" \
				} \
			}, "trigonometric functions");
			addTab(new TabQWidget{ \
				new Grid::GridCalculateKeyboard<Button::ButtonDrag>{ \
					std::vector<std::vector<const char *>>{
						{"sgn", "abs", "mod"}
					}, window, "operator" \
				} \
			}, "other functions");
			addTab(new TabQWidget{ \
				new Grid::GridCalculateKeyboard<Button::ButtonDrag>{ \
					std::vector<std::vector<const char *>>{
						{"0x", "0b", "0t"}
					}, window, "prefix" \
				} \
			}, "number system");
			tabBar()->setElideMode(Qt::ElideNone);
		}
	};



	//Main TabWidget

	class MainTabWidget : public QTabWidget {
	public:
		explicit MainTabWidget( \
			Window *window \
		) : QTabWidget() {
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
			tabBar()->setElideMode(Qt::ElideNone);
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
		Title::TitleBar * const titleBar \
			{new Title::TitleBar{app, window}};
		addWidget(titleBar);
		CreateHistori::HistoriScroll *globalHistori \
			{new CreateHistori::HistoriScroll{}};
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
		addWidget(new TabWindow::TabWidgetKeyboard{window});
		titleBar->getChild()->buttonInit();
		puts("jk");
		setStretch(0, 5);
		setStretch(1, 30);
		setStretch(2, 50);
		setStretch(3, 15);
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