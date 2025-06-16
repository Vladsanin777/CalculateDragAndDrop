#pragma once
#include <QPainterPath>
#include <QCheckBox>
#include <QSlider>
#include <QPushButton>
#include <QAbstractButton>
#include <QPropertyAnimation>
#include <QApplication>
#include <QShortcut>
#include <QMainWindow>
#include <QWidget>
#include <QPointF>
#include <QVBoxLayout>
#include <QBoxLayout>
#include <QScrollArea>
#include <QScrollBar>
#include <QLineEdit>
#include <QString>
#include <QFocusEvent>
#include <QDrag>
#include <QDockWidget>
#include <QMimeData>
#include <QTabBar>
#include <QTimer>
#include <QTabWidget>
#include <QLabel>
#include <QStyle>
#include <QSizePolicy>
#include <QPainter>

#include <functional>

#define COUNT_LOCAL_HISTORI 5

using byte = unsigned char;

int fontSizeSettingLabel {20}, fontSizeLabelRGB {12}, \
	fontSizeButtonKeyboard{18}, fontSizeLabelCalc{15}, \
	fontSizeButtonHide{14}, fontSizeTitleButton{20}, \
	fontSizeButtonSetting{18}, fontSizeButtonKeyboardExstra{18};

enum MODS {BASIC, DERIVATIVE, INTEGRATE, INTEGRAL, REPLACEMENT};
#define LIGHT 255, 255, 255, 150, 150, 150
#define DARK 150, 150, 150, 0, 0, 0

#define LIGHT_RED 219, 0, 0, 87, 0, 0
#define LIGHT_ORANGE 244, 129, 8, 116, 62, 4
#define LIGHT_YELLO 255, 193, 2, 125, 95, 0

#define DARK_RED 87, 0, 0, 10, 0, 0
#define DARK_ORANGE 116, 62, 4, 42, 22, 1
#define DARK_YELLO 125, 95, 0, 48, 36, 0

#define LIGHT_GREEN 81, 171, 42, 46, 65, 16
#define LIGHT_BLUE 49, 118, 238, 17, 61, 138
#define LIGHT_PURPLE 77, 41, 170, 29, 15, 64

#define DARK_GREEN 46, 65, 16, 2, 3, 1
#define DARK_BLUE 17, 61, 138, 8, 30, 70
#define DARK_PURPLE 29, 15, 64, 1, 0, 2

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
	columnConsts {byte(3)};
const char * const consts[rowConsts][columnConsts] {
	{"e", "pi", "x"} \
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
	{"pi", "e"} \
};
const byte rowEmptyMain{byte(1)}, \
	columnEmptyMain{byte(5)};
const char * const emptyMain[rowEmptyMain] \
	[columnEmptyMain] {
	{"", "", "", "", ""} \
};

namespace RGB {
	class RGB;
	class RGBS;
	class Slider;
	class Channel;
	class Color;
	class Gradient;
}

namespace Application {
	class CalculateDragAndDrop;
}
namespace Window {
	class Window;
}
namespace Label {
	class Label;
}
namespace Button {
	class ButtonBase;
	class ButtonDrag;
	class ButtonDragAndDrop;
	class ButtonTheme;
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
	class SettingGrid;
	class SettingWidget;
	class SettingArea;
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
namespace RGB {
	class RGBA {
	private:
		byte _red{byte(0)}, _green{byte(0)}, \
			_blue{byte(0)}, _alpha{byte{0}};
		//std::function<void(byte, byte, byte)> _func;
	public:
		inline explicit RGBA(byte red, byte green, \
			byte blue, byte alpha) noexcept;
		inline void set(byte red, byte green, \
			byte blue, byte alpha) noexcept;
		
		//inline void setFunc( \
			std::function<void(byte, byte, byte)> func \
		) noexcept;
		inline byte red(void) const noexcept;
		inline byte green(void) const noexcept;
		inline byte blue(void) const noexcept;
		inline byte alpha(void) const noexcept;
	};
	class PRGBA : public RGBA {
	private:
		double _pointer {0.0};
	public:
		inline explicit PRGBA(double pointer, \
			byte red, byte green, \
			byte blue, byte alpha) noexcept;
		inline byte pointer(void) const noexcept;
	};

	class PRGBAS {
	private:
		std::vector<PRGBA> _drgbas {};
	public:
		inline explicit PRGBAS( \
			const std::vector<PRGBA> drgbas \
		) noexcept;
		PRGBA &operator[]( \
			size_t index) noexcept;
	};
	class Slider : public QSlider {
	public:
		inline explicit Slider(byte number, \
			std::function<void(void)> func, \
			Channel *parent) noexcept;
	};
	class Channel : public QBoxLayout {
	private:
		Label::Label * _label;
		Slider * _slider;
	public:
		inline explicit Channel(byte number, \
			std::function<void(void)> func) noexcept;
		inline Slider* slider(void) noexcept;
		inline Label::Label* label(void) noexcept;
	};
	class Color : public QBoxLayout {
	private:
		Channel * _redC{nullptr}, *_greenC{nullptr}, \
		*_blueC{nullptr};
		Slider * _red{nullptr}, * _green{nullptr}, \
		* _blue{nullptr};
	public:
		inline explicit Color(Window::Window *window, \
			const RGB& rgb, \
			std::function<void(void)> funcApplyValue, \
			std::function<void( \
            std::function<void(byte, byte, byte)>)> func\
		) noexcept;
		inline void setValue( \
			byte red, byte green, byte blue) noexcept;
		inline byte getRedValue(void) noexcept;
		inline byte getGreenValue(void) noexcept;
		inline byte getBlueValue(void) noexcept;

	};
	class Gradient : public QGridLayout {
	private:
		Color *_first{nullptr}, *_second{nullptr};
		Window::Window * _window;
	public:
		inline explicit Gradient(Window::Window *window) noexcept;
		inline void applyGradient(void) noexcept;
	};
}
namespace Application {
	class CalculateDragAndDrop : public QApplication {
	public:
		inline explicit CalculateDragAndDrop( \
			int argc, char *argv[]) noexcept;
	};
}
namespace Window {
	class Window : public QMainWindow {
	private:
		RGB::RGB _rgb0 {0, 0, 0}, _rgb1{0, 0, 0};
		MODS _currentMod {BASIC};
		byte _currentIndex {byte(0)};
		Setting::SettingDock * _settingDock {nullptr};
		LineEdit::LineEdit * _lineEditLongArifmetic {nullptr};
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
			MODS mod) const noexcept;
		inline CreateHistori::HistoriScroll* getLocalHistori( \
			void) const noexcept;
		inline void setLocalHistori( \
		CreateHistori::HistoriScroll* newLocalHistori, \
			MODS mod) noexcept;
		inline CreateHistori::HistoriWidget* getResizeLocalHistori( \
			MODS mod) const noexcept;
		inline CreateHistori::HistoriWidget* getResizeLocalHistori( \
			void) const noexcept;
		inline void setResizeLocalHistori( \
		CreateHistori::HistoriWidget* newResizeLocalHistori, \
			MODS mod) noexcept;
		inline CreateHistori::HistoriVBox* getAddLocalHistori( \
			MODS mod) const noexcept;
		inline CreateHistori::HistoriVBox* getAddLocalHistori( \
			void) const noexcept;
		inline void setAddLocalHistori( \
			CreateHistori::HistoriVBox* newAddLocalHistori, \
			MODS mod) noexcept;
		inline LineEdit::LineEdit* getLineEdit( \
			MODS mod, byte index) const noexcept;
		inline LineEdit::LineEdit* getLineEdit(void \
		) const noexcept;
		inline void setLineEdit(LineEdit::LineEdit* newLineEdit, \
			MODS mod, byte index) noexcept;
		inline MODS getMod(void) const noexcept;
		inline void setMod(const MODS mod) noexcept;
		inline byte getIndex(void) const noexcept;
		inline void setIndex(const byte index) noexcept;
		inline Button::ButtonDrag* getResultButton() noexcept;
		inline void setResultButton( \
			Button::ButtonDrag* resultButton) noexcept;
		inline const char *getResult( \
			MODS mod, byte index) const noexcept;
		inline const char *getResult(void) const noexcept;
		inline void setResult( \
			const char *newResult, MODS mod, byte index \
		) noexcept;
		inline void setResult(const char *newResult \
		) noexcept;
		template<typename TDel>
		inline void deleteResultOrLineEdit(TDel ptr \
		) const noexcept;
		inline void updataResultButton(void) const noexcept;
		inline void updataResultButton(MODS mod, byte index \
		) const noexcept;
		inline void setLineEditLongArifmetic( \
			LineEdit::LineEdit * lineEdit) noexcept;
		inline LineEdit::LineEdit * getLineEditLongArifmetic( \
			void) const noexcept;
		inline void setSettingDock( \
			Setting::SettingDock * settingDock) noexcept;
		inline Setting::SettingDock * getSettingDock( \
			void) const noexcept;
		inline void setRGB( \
			byte red0, byte green0, byte blue0, \
			byte red1, byte green1, byte blue1 \
		) noexcept;
		inline void setRGB( \
			RGB::RGB rgb0, RGB::RGB rgb1 \
		) noexcept;
		inline const RGB::RGB& getRGB0(void) noexcept;
		inline const RGB::RGB& getRGB1(void) noexcept;
		inline void setFuncRGB0(std::function<void(byte, \
			byte, byte)> func) noexcept;
		inline void setFuncRGB1(std::function<void(byte, \
			byte, byte)> func) noexcept;
	protected:
		inline void paintEvent(QPaintEvent *event) override;
	private:
		inline ~Window(void);
	};
}



namespace Label {
	class Label : public QLabel {
	public:
		inline explicit Label(const char * label, \
			int fontSize) noexcept;
		inline explicit Label(byte number, \
			int fontSize) noexcept;
	};
}
namespace Button {
	class ButtonBase : public QPushButton {
	private:
		Window::Window *_window {nullptr};
	public:
		inline explicit ButtonBase( \
			const char *label, Window::Window *window, \
			std::function<void(QPushButton *)> *callback, \
			int fontSize, const char *cssName = "basic" \
		) noexcept;
		inline explicit ButtonBase( \
			const char *label, Window::Window *window, \
			std::function<void(void)> *callback, \
			int fontSize, const char *cssName = "basic" \
		) noexcept;
		inline explicit ButtonBase( \
			const char *label, Window::Window *window, \
			int fontSize, const char *cssName = "basic" \
		) noexcept;
	};
	class ButtonDrag : public ButtonBase {
	private:
		QPoint _start_pos;
	public:
		explicit ButtonDrag( \
			const char *label, Window::Window *window, \
			std::function<void(QPushButton *)> *callback, \
			int fontSize, const char *cssName = nullptr \
		);
	protected:
		void mousePressEvent(QMouseEvent *event) override;
		void mouseMoveEvent(QMouseEvent *event) override;
	};
	class ButtonDragAndDrop : public ButtonDrag {
	public:
		explicit inline ButtonDragAndDrop( \
			const char *label, Window::Window *window, \
			std::function<void(QPushButton *)> *callback, \
			int fontSize, const char *cssName = nullptr \
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
	class ButtonTheme : public ButtonBase {
	private:
		RGB::RGB _rgb0{0, 0, 0}, _rgb1{0, 0, 0};
	public:
		inline explicit ButtonTheme(Window::Window * window, \
			byte red0, byte green0, byte blue0, \
			byte red1, byte green1, byte blue1 \
		) noexcept;
	protected:
		inline void paintEvent(QPaintEvent * event \
		) noexcept override;
	};
}
namespace LineEdit {
	class LineEdit : public QLineEdit {
	private:
        Window::Window *_window;
        MODS _mod {BASIC};
		byte _index {byte(0)};
    public:
        explicit LineEdit ( \
            Window::Window *window, MODS mod, byte index, \
            const char *text = "" \
        );
        explicit LineEdit ( \
            Window::Window *window, const char *text = "" \
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
		MODS _mod {BASIC};
		byte _index{byte(0)};
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
	inline void visibleSetting(Window::Window * window, \
		QPushButton * button) noexcept;
	inline void changeHistoriVisible( \
		Window::Window * window, QPushButton * button \
	) noexcept;
	inline void applyLongArifmetic(Window::Window * window, \
		QPushButton* button) noexcept;
	inline void setRGB(Window::Window * window, \
		byte red0, byte green0, byte blue0, \
		byte red1, byte green1, byte blue1 \
		) noexcept;
	inline void createWindow(Application::\
		CalculateDragAndDrop * app) noexcept;
}
namespace Title {
	class TitleLayout : public QHBoxLayout {
	public:
		explicit TitleLayout( \
			Application::CalculateDragAndDrop *app, \
			Window::Window *window \
		) noexcept;
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
	public:
		inline explicit SettingGrid(Window::Window * window, \
			QWidget * parent = nullptr) noexcept;

	};
	class SettingWidget : public QWidget {
	public:
		inline explicit SettingWidget(Window::Window * window, \
			QWidget * parent = nullptr) noexcept;

	};
	class SettingArea : public QScrollArea {
	public:
		inline explicit SettingArea(Window::Window * window, \
		QWidget * parent = nullptr) noexcept;
		void setWidget(QWidget *widget) noexcept;
		inline bool eventFilter(QObject *obj, \
			QEvent *event) noexcept override;

	protected:
		inline void resizeEvent(QResizeEvent *event \
		) noexcept override;

	private:
		inline void updateMinimumWidth(void) noexcept;
	};
	class SettingDock : public QDockWidget {
	private:
		const RGB::RGB& _rgb0, &_rgb1;
	public:
		inline explicit SettingDock(Window::Window * window \
		) noexcept;
	protected:
		inline void paintEvent(QPaintEvent *event) override;
	};
}

namespace Grid {
	template<class TButton, byte rowSize, byte columnSize>
	inline void buildingGridKeyboard( \
		const char * const (&buttons)[rowSize][columnSize], \
		QGridLayout *grid, Window::Window *window, \
		int fontSizeButton, \
		const byte rowStart = byte(0), \
		const byte columnStart = byte(0), \
		const char * cssName = "opertor" \
	) noexcept;
	template<class TButton, byte rowSize, byte columnSize>
	class GridCalculateKeyboard : public QGridLayout {
	public:
		inline explicit GridCalculateKeyboard( \
			const char * const (&buttons)[rowSize][columnSize], \
			Window::Window *window, int fontSize, \
			const char * cssName \
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
			Window::Window *window, MODS mod \
		);
	};
	class GridDefaultCalc : public GridBaseCalc {
	public:
		inline explicit GridDefaultCalc( \
			Window::Window *window, MODS mod \
		);
	};
	class GridIntegralCalc : public GridBaseCalc {
	public:
		inline explicit GridIntegralCalc( \
			Window::Window *window, MODS mod = INTEGRAL \
		);
	};
	class GridReplacementCalc : public GridBaseCalc {
	public:
		inline explicit GridReplacementCalc( \
			Window::Window *window, MODS mod = REPLACEMENT \
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
#include "Setting.hpp"
#include "Label.hpp"
#include "RGB.hpp"