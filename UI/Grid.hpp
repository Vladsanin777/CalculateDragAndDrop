
#pragma once
#include "UI.hpp"


namespace Grid {
	template<class TButton, byte rowSize, byte columnSize>
	inline void buildingGridKeyboard( \
		const char * const (&buttons)[rowSize][columnSize], \
		QGridLayout *grid, Window::Window *window, \
		const byte rowStart, const byte columnStart, \
		const char * cssName \
	) noexcept {
		std::function<void(QPushButton *)> * func { \
			new std::function<void(QPushButton*)>{
				[window](QPushButton* btn) {
					LogicButton::LogicCalculate:: \
						inputtinLineEdit(btn, window);
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
	inline GridCalculateKeyboard<TButton, rowSize, columnSize>:: \
	GridCalculateKeyboard( \
		const char * const (&buttons)[rowSize][columnSize], \
		Window::Window *window, const char * cssName \
	) noexcept : QGridLayout() {
		setContentsMargins(10, 10, 10, 10);
		setSpacing(10);
		buildingGridKeyboard<TButton, rowSize, columnSize>( \
			buttons, this, window, byte(0), \
			byte(0), cssName \
		);
	}
	inline GridCalculateCommon::GridCalculateCommon( \
		Window::Window *window) : QGridLayout{} {
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

	inline GridBaseCalc::GridBaseCalc( \
		Window::Window *window, byte tab \
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

	inline GridDefaultCalc::GridDefaultCalc( \
		Window::Window *window, byte tab \
	) : GridBaseCalc(window, tab) {
		LineEdit::LineEdit *lineEdit \
			{new LineEdit::LineEdit{window, tab, byte(0)}};
		window->setLineEdit(lineEdit, tab, byte(0));
		addWidget(lineEdit, 1, 0, 1, 6);
	}

	inline GridIntegralCalc::GridIntegralCalc( \
		Window::Window *window, byte tab \
	) : GridBaseCalc(window, tab) {
		addWidget( \
			new Button::ButtonBase{ \
				"a = ", window, nullptr \
			}, 1, 0, 1, 1 \
		);
		LineEdit::LineEdit *aLineEdit \
			{new LineEdit::LineEdit{window, 1, 0, "1"}};
		window->setLineEdit(aLineEdit, tab, byte(0));
		addWidget(aLineEdit, 1, 1, 1, 2);
		addWidget( \
			new Button::ButtonBase{ \
				"b = ", window, nullptr \
			}, 1, 3, 1, 1 \
		);
		LineEdit::LineEdit *bLineEdit \
			{new LineEdit::LineEdit{window, tab, byte(1), "2"}};
		window->setLineEdit(bLineEdit, tab, byte(1));
		addWidget(bLineEdit, 1, 4, 1, 2);
		LineEdit::LineEdit *mainLineEdit \
			{new LineEdit::LineEdit{window, tab, byte(2)}};
		window->setLineEdit(mainLineEdit, tab, byte(2));
		addWidget(mainLineEdit, 2, 0, 1, 6);
	}
	inline GridReplacementCalc::GridReplacementCalc( \
		Window::Window *window, byte tab \
	) : GridBaseCalc{window, tab} {
		addWidget( \
			new Button::ButtonBase{ \
				"with =", window, \
				nullptr \
			}, 1, 0, 1, 1 \
		);
		LineEdit::LineEdit *withLineEdit = \
			new LineEdit::LineEdit(window, tab, byte(0), "x");
		window->setLineEdit(withLineEdit, tab, byte(0));
		addWidget(withLineEdit, 1, 1, 1, 2);
		addWidget( \
			new Button::ButtonBase( \
				"on =", window, \
				nullptr \
			), 1, 3, 1, 1 \
		);
		LineEdit::LineEdit *onLineEdit \
			{new LineEdit::LineEdit{window, tab, byte(1), "0"}};
		window->setLineEdit(onLineEdit, tab, byte(1));
		addWidget(onLineEdit, 1, 4, 1, 2);
		LineEdit::LineEdit *mainLineEdit \
			{new LineEdit::LineEdit{window, tab, byte(2)}};
		window->setLineEdit(mainLineEdit, tab, byte(2));
		addWidget(mainLineEdit, 2, 0, 1, 6);
	}
}