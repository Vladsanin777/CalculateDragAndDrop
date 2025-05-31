#pragma once
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
#include <QPainter>

#include <functional>

#define COUNT_LOCAL_HISTORI 5

using byte = unsigned char;


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
	{"!", "sqrt(", "ln(", "log", "lg("} \
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


namespace Application {
	class CalculateDragAndDrop;
}
namespace Window {
	class Window;
}
namespace Button {
	class ButtonBase;
	class ButtonDrag;
	class ButtonDragAndDrop;
}
namespace LineEdit {
	class LineEdit;
}
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
namespace LogicButton {
	class LogicCalculate;
}
namespace Title {
	class TitleLayout;
	class TitleBar;
}
namespace Setting {
	class SettingWidget;
	class SettingDock;
}
namespace Grid {
	template<class TButton, byte rowEnd, byte columnEnd>
	class GridCalculateKeyboard;
	class GridCalculateCommon;
	class GridBaseCalc;
	class GridDefaultCalc;
	class GridIntegralCalc;
	class GridReplacementCalc;
}
namespace TabWidget {
	class TabQWidget;
	class TabWidgetKeyboard;
	class MainTabWidget;
}
namespace MainWidget {
	class MainLayout;
	class MainWidget;
}

namespace Application {
	class CalculateDragAndDrop : public QApplication {
	public:
		inline explicit CalculateDragAndDrop( \
			int argc, char *argv[] \
		);
		inline void createWindow(QPushButton *button);
	};
}
namespace Window {
	class Window : public QMainWindow {
	private:
		size_t _longArifmetic {256UL};
		CreateHistori::HistoriScroll ** const _localHistori \
			{new CreateHistori::HistoriScroll*[COUNT_LOCAL_HISTORI]{nullptr}};
		CreateHistori::HistoriWidget ** const _resizeLocalHistori \
			{new CreateHistori::HistoriWidget*[COUNT_LOCAL_HISTORI]{nullptr}};
		CreateHistori::HistoriVBox ** const _addLocalHistori \
			{new CreateHistori::HistoriVBox*[COUNT_LOCAL_HISTORI]{nullptr}};
		LineEdit::LineEdit ** const * const _lineEdit { \
			new LineEdit::LineEdit ** const [COUNT_LOCAL_HISTORI] { \
				new LineEdit::LineEdit * [1]{nullptr}, \
				new LineEdit::LineEdit * [1]{nullptr}, \
				new LineEdit::LineEdit * [1]{nullptr}, \
				new LineEdit::LineEdit * [3]{nullptr, \
					nullptr, nullptr}, \
				new LineEdit::LineEdit * [3]{nullptr, \
					nullptr, nullptr} \
			}
		};
		const char ** const * const _result { \
			new const char ** const [5]{ \
				{new const char * [1]{new const char [] {"0"}}}, \
				{new const char * [1]{new const char [] {"0"}}}, \
				{new const char * [1]{new const char [] {"0"}}}, \
				{new const char * [3]{new const char [] {"1"}, \
					new const char [] {"2"}, new const char [] {"0"}}}, \
				{new const char * [3]{new const char [] {"x"}, \
					new const char [] {"0"}, new const char [] {"0"}}} \
			} \
		};
		byte * const _inputtin {new byte [2]{0, 0}};
		Button::ButtonDrag *_resultButton {nullptr};
		CreateHistori::HistoriScroll *_globalHistori {nullptr};
		CreateHistori::HistoriVBox *_addGlobalHistori {nullptr};
		CreateHistori::HistoriWidget *_resizeGlobalHistori {nullptr};
		Application::CalculateDragAndDrop *_app {nullptr};
		//Setting *_setting {nullptr};
	public:
		explicit inline Window( \
			Application::CalculateDragAndDrop *app = nullptr \
		);
		inline CreateHistori::HistoriScroll* getGlobalHistori( \
			void) const noexcept;
		inline void setGlobalHistori( \
			CreateHistori::HistoriScroll* newGlobalHistori \
		) noexcept;
		inline CreateHistori::HistoriWidget* getResizeGlobalHistori( \
			void) const noexcept;
		inline void setResizeGlobalHistori( \
			CreateHistori::HistoriWidget* newResizeGlobalHistori \
		) noexcept;
		inline CreateHistori::HistoriVBox* getAddGlobalHistori( \
			void) const noexcept;
		inline void setAddGlobalHistori( \
			CreateHistori::HistoriVBox* newAddGlobalHistori \
		) noexcept;
		inline CreateHistori::HistoriScroll* getLocalHistori( \
			byte index) const noexcept;
		inline CreateHistori::HistoriScroll* getLocalHistori( \
			void) const noexcept;
		inline void setLocalHistori( \
		CreateHistori::HistoriScroll* newLocalHistori, \
			byte tab) noexcept;
		inline CreateHistori::HistoriWidget* getResizeLocalHistori( \
			byte index) const noexcept;
		inline CreateHistori::HistoriWidget* getResizeLocalHistori( \
			void) const noexcept;
		inline void setResizeLocalHistori( \
		CreateHistori::HistoriWidget* newResizeLocalHistori, \
			byte index) noexcept;
		inline CreateHistori::HistoriVBox* getAddLocalHistori( \
			byte index) const noexcept;
		inline CreateHistori::HistoriVBox* getAddLocalHistori( \
			void) const noexcept;
		inline void setAddLocalHistori( \
			CreateHistori::HistoriVBox* newAddLocalHistori, \
			byte index) noexcept;
		inline LineEdit::LineEdit* getLineEdit( \
			byte tab, byte index) const noexcept;
		inline LineEdit::LineEdit* getLineEdit(void \
		) const noexcept;
		inline void setLineEdit(LineEdit::LineEdit* newLineEdit, \
			byte tab, byte index) noexcept;
		inline const byte* getInputtin() const noexcept;
		inline void setInputtin(const byte &tab, \
			const byte &index) noexcept;
		inline Button::ButtonDrag* getResultButton() noexcept;
		inline void setResultButton( \
			Button::ButtonDrag* resultButton) noexcept;
		inline const char *getResult( \
			byte tab, byte index) const noexcept;
		inline const char *getResult(void) const noexcept;
		inline void setResult( \
			const char *newResult, byte tab, byte index \
		) noexcept;
		inline void setResult(const char *newResult \
		) noexcept;
		template<typename TDel>
		inline void deleteResultOrLineEdit(TDel ptr \
		) const noexcept;
		void updataResultButton(void) const noexcept;
		void updataResultButton(byte tab, byte index \
		) const noexcept;
	protected:
		void paintEvent(QPaintEvent *event) override;
	private:
		inline ~Window(void);
	};
}
namespace Button {
	class ButtonBase : public QPushButton {
	private:
		Window::Window *_window {nullptr};
	public:
		inline explicit ButtonBase( \
			const char *label, Window::Window *window, \
			std::function<void(QPushButton *)> \
			*callback = nullptr, \
			const char *cssName = "basic" \
		);
	};
	class ButtonDrag : public ButtonBase {
	private:
		QPoint _start_pos;
	public:
		explicit ButtonDrag( \
			const char *label, Window::Window *window, \
			std::function<void(QPushButton *)> *callback = \
			nullptr, const char *cssName = nullptr \
		);
	protected:
		void mousePressEvent(QMouseEvent *event) override;
		void mouseMoveEvent(QMouseEvent *event) override;
	};
	class ButtonDragAndDrop : public ButtonDrag {
	public:
		explicit inline ButtonDragAndDrop( \
			const char *label, Window::Window *window, \
			std::function<void(QPushButton *)> *callback = \
			nullptr, const char *cssName = nullptr \
		);
		static inline bool isEmpty(const char * const &text);
		static inline bool isNumber(const char * const &text);
		static inline bool isPrefix(const char * const &text);
		static inline bool isConst(const char * const &text);
		static inline bool isBracket(const char * const &text);
		static inline bool isFunction(const char * const &text);
		static inline bool is1Operator(const char * const &text);
		static inline bool is2Operator(const char * const &text);
		static inline bool is3Operator(const char * const &text);
		static inline bool is4Operator(const char * const &text);
		static inline bool isOperator(const char * const &text);
	protected:
		inline void dragEnterEvent(QDragEnterEvent *event \
		) override;
		inline void dropEvent(QDropEvent *event) override;
	};
}
namespace LineEdit {
	class LineEdit : public QLineEdit {
	private:
        Window::Window *_window;
        byte _tab {byte(0)}, _index {byte(0)};
    public:
        explicit LineEdit ( \
            Window::Window *window, byte tab, byte index, \
            const char *text = "" \
        );
    protected:
        void focusInEvent(QFocusEvent *event) override;
	private:
        void onLineEditChanged( \
            const QString& text \
        ) const noexcept;
	};
}
namespace NewHistoriElement {
	class LabelHistori : public QLabel {
	private:
		Window::Window *_window {nullptr};
		const char *_callback {nullptr};
	public:
		explicit inline LabelHistori( \
			const char* label, const char *cssName, \
			Window::Window *window \
		);
		explicit inline LabelHistori( \
			const char* label, const char *cssName, \
			Window::Window *window, const char *customCallback \
		);
	protected:
		inline void mousePressEvent(QMouseEvent* event \
		) noexcept override;
	private:
		~LabelHistori(void);
	};
	class BaseBoxHistoriElement : public QHBoxLayout {
	public:
		explicit inline BaseBoxHistoriElement( \
			const char * expression, Window::Window *window, \
			const char * result
		) noexcept;
	};
	class BasicBoxHistoriElement : public QVBoxLayout {
	public:
		explicit inline BasicBoxHistoriElement( \
			const char * expression, Window::Window * window, \
			const char * result, const char * nameOperation \
		) noexcept;
	};
	class SubCustomBoxHistoriElement : public QHBoxLayout {
	public:
		explicit inline SubCustomBoxHistoriElement( \
			Window::Window * window, \
			const char * label1, const char * text1, \
			const char * label2, const char * text2 \
		) noexcept;
	};
	class CustomBoxHistoriElement : public QVBoxLayout {
	public:
		explicit inline CustomBoxHistoriElement( \
			const char * const expression, \
			const char * const result, \
			Window::Window * const  window, byte tab, \
			const char * const label1, \
			const char * const text1, \
			const char * const label2, \
			const char * const text2 \
		) noexcept;
		explicit inline CustomBoxHistoriElement( \
			const char * const expression, \
			const char * const result, \
			Window::Window * const  window, byte tab, \
			const char * const label1, \
			const char * const text1, \
			const char * const label2, \
			const char * const text2, \
			const char * const  nameOperation \
		) noexcept;
	};
}
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

namespace LogicButton {
	class LogicCalculate {
	private:
		char *_lineEditText {nullptr};
		Window::Window *_window {nullptr};
		byte _tab {byte(0)}, _index{byte(0)};
	public:
        explicit LogicCalculate( \
            char *lineEditText, Window::Window *window \
        );
        inline bool isMainTab(void);
        inline bool isMainLineEdit(void);
        inline void button_ALL(void);
        inline void button_DO(void);
        inline void button_POS(void);
        inline void button_RES(void);
        inline void addHistori(void);
        inline void button_O(void);
        inline void buttonOther(void);
        inline static void inputtinLineEdit( \
            QPushButton *button, Window::Window *window \
        );
	};
	void applyLongArifmetic(Window::Window * window, \
		QPushButton* button) noexcept;
}
namespace Title {
	class TitleLayout : public QHBoxLayout {
	private:
		const Window::Window         *_window                  {nullptr};
		CreateHistori::HistoriScroll *_globalHistori           {nullptr};
		CreateHistori::HistoriScroll *_localHistoriBasic       {nullptr};
		CreateHistori::HistoriScroll *_localHistoriIntegral    {nullptr};
		CreateHistori::HistoriScroll *_localHistoriDerivative  {nullptr};
		CreateHistori::HistoriScroll *_localHistoriIntegrate   {nullptr};
		CreateHistori::HistoriScroll *_localHistoriReplacement {nullptr};
		Button::ButtonBase           *_buttonChangeHistori     {nullptr};
	public:
		explicit TitleLayout( \
			Application::CalculateDragAndDrop *app, \
			Window::Window *window \
		) noexcept;
		void buttonInit(void);
		void changeHistoriVisible(QPushButton *button) const;
	};
	class TitleBar : public QWidget {
	private:
		TitleLayout *_child {nullptr};
	public:
		explicit TitleBar( \
			Application::CalculateDragAndDrop *app, \
			Window::Window *window \
		) noexcept;
		TitleLayout *getChild(void) noexcept;
	};
}

namespace Setting {
	class SettingGrid : public QGridLayout {
		inline explicit SettingGrid(Window::Window * window \
		) noexcept;
	}
	class SettingWidget : public QWidget {
		inline explicit SettingWidget(Window::Window * window \
		) noexcept;
	};
	class SettingDock : public QDockWidget {
		inline explicit SettingDock(Window::Window * window \
		) noexcept;
	};
}

namespace Grid {
	template<class TButton, byte rowSize, byte columnSize>
	inline void buildingGridKeyboard( \
		const char * const (&buttons)[rowSize][columnSize], \
		QGridLayout *grid, Window::Window *window, \
		const byte rowStart = byte(0), \
		const byte columnStart = byte(0), \
		const char * cssName = "opertor" \
	) noexcept;
	template<class TButton, byte rowSize, byte columnSize>
	class GridCalculateKeyboard : public QGridLayout {
	public:
		inline explicit GridCalculateKeyboard( \
			const char * const (&buttons)[rowSize][columnSize], \
			Window::Window *window, const char * cssName \
		) noexcept;
	};
	class GridCalculateCommon : public QGridLayout {
	public:
		inline explicit GridCalculateCommon( \
			Window::Window *window \
		);
	};
	class GridBaseCalc : public QGridLayout {
	public:
		inline explicit GridBaseCalc( \
			Window::Window *window, byte tab \
		);
	};
	class GridDefaultCalc : public GridBaseCalc {
	public:
		inline explicit GridDefaultCalc( \
			Window::Window *window, byte tab \
		);
	};
	class GridIntegralCalc : public GridBaseCalc {
	public:
		inline explicit GridIntegralCalc( \
			Window::Window *window, byte tab = byte(3) \
		);
	};
	class GridReplacementCalc : public GridBaseCalc {
	public:
		inline explicit GridReplacementCalc( \
			Window::Window *window, byte tab = byte(4) \
		);
	};
}

namespace TabWidget {
	class TabQWidget : public QWidget {
	public:
		inline explicit TabQWidget(QGridLayout *tab);
	};

	class TabWidgetKeyboard : public QTabWidget {
	public:
		inline explicit TabWidgetKeyboard(Window::Window *window);
	};

	class MainTabWidget : public QTabWidget {
	public:
		inline explicit MainTabWidget(Window::Window *window);
	};
}
namespace MainWidget {
	class MainLayout : public QVBoxLayout {
	public:
		inline explicit MainLayout( \
			Application::CalculateDragAndDrop *app, \
			Window::Window *window \
		) noexcept;
	};
	class MainWidget : public QWidget {
	public:
		inline explicit MainWidget ( \
			Application::CalculateDragAndDrop * app, \
			Window::Window *window \
		) noexcept;
	};
}

#include "Application.hpp"
#include "Button.hpp"
#include "CreateHistori.hpp"
#include "Grid.hpp"
#include "LineEdit.hpp"
#include "LogicButton.hpp"
#include "MainWidget.hpp"
#include "NewHistoriElement.hpp"
#include "TabWidget.hpp"
#include "Title.hpp"
#include "Window.hpp"