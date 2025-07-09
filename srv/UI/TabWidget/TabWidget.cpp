#pragma once
#include "TabWidget.hpp"


namespace TabWidget {
	inline TabQWidget::TabQWidget( \
		QGridLayout *tab \
	) : QWidget() {
		setLayout(tab);
	}


	inline TabWidgetKeyboard::TabWidgetKeyboard( \
		Window::Window *window \
	) : QTabWidget() {
		addTab(new TabQWidget{ \
			new Grid::GridCalculateKeyboard< \
				Button::ButtonDrag, rowDigits10, \
				columnDigits10>{ \
				digits10, window, fontSizeButtonKeyboardExstra, \
				"number" \
			} \
		}, "digits 10");
		addTab(new TabQWidget{ \
			new Grid::GridCalculateKeyboard< \
				Button::ButtonDrag, rowDigits16, \
				columnDigits16>{ \
				digits16, window, fontSizeButtonKeyboardExstra, \
				"number" \
			} \
		}, "digits 16");
		addTab(new TabQWidget{ \
			new Grid::GridCalculateKeyboard< \
				Button::ButtonDrag, rowOperators1, \
				columnOperators1>{ \
				operators1, window, fontSizeButtonKeyboardExstra, \
				"operator" \
			} \
		}, "operators 1");
		addTab(new TabQWidget{ \
			new Grid::GridCalculateKeyboard< \
				Button::ButtonDrag, rowOperators2, \
				columnOperators2>{ \
				operators2, window, fontSizeButtonKeyboardExstra, \
				"operator" \
			} \
		}, "operators 2");
		addTab(new TabQWidget{ \
			new Grid::GridCalculateKeyboard< \
				Button::ButtonDrag, rowBrackets, \
				columnBrackets>{ \
				brackets, window, fontSizeButtonKeyboardExstra, \
				"bracket" \
			} \
		}, "brackets");
		addTab(new TabQWidget{ \
			new Grid::GridCalculateKeyboard< \
				Button::ButtonDrag, rowBraces, columnBraces>{ \
				braces, window, fontSizeButtonKeyboardExstra, \
				"bracket" \
			} \
		}, "braces");
		addTab(new TabQWidget{ \
			new Grid::GridCalculateKeyboard< \
				Button::ButtonDrag, rowConsts, columnConsts>{ \
				consts, window, fontSizeButtonKeyboardExstra, \
				"const" \
			} \
		}, "consts");
		addTab(new TabQWidget{ \
			new Grid::GridCalculateKeyboard< \
				Button::ButtonDrag, rowTrigonometricFuncs, \
				columnTrigonometricFuncs>{ \
				trigonometricFuncs, window, \
				fontSizeButtonKeyboardExstra, \
				"operator" \
			} \
		}, "trigonometric functions");
		addTab(new TabQWidget{ \
			new Grid::GridCalculateKeyboard< \
				Button::ButtonDrag, \
				rowInverseTrigonometricFuncs, \
				columnInverseTrigonometricFuncs>{
				inverseTrigonometricFuncs, \
				window, fontSizeButtonKeyboardExstra, \
				"operator" \
			} \
		}, "inverse trigonometric functions");
		addTab(new TabQWidget{ \
			new Grid::GridCalculateKeyboard< \
				Button::ButtonDrag, rowPrefixes, \
				columnPrefixes>{ \
				prefixes, window, fontSizeButtonKeyboardExstra, \
				"prefix" \
			} \
		}, "prefix");
		addTab(new TabQWidget{ \
			new Grid::GridCalculateKeyboard< \
				Button::ButtonDrag, rowFunctions, \
				columnFunctions>{ \
				functions, window, fontSizeButtonKeyboardExstra, \
				"function" \
			} \
		}, "functions");
		tabBar()->setElideMode(Qt::ElideNone);
	}


	inline MainTabWidget::MainTabWidget( \
		Window::Window *window) : QTabWidget() {
		addTab(new TabQWidget{ \
			new Grid::GridDefaultCalc{window, BASIC} \
		}, lstTabs[0]);
		addTab(new TabQWidget{ \
			new Grid:: \
				GridDefaultCalc{window, DERIVATIVE} \
		}, lstTabs[1]);
		addTab(new TabQWidget{ \
			new Grid:: \
				GridDefaultCalc{window, INTEGRATE} \
		}, lstTabs[2]);
		addTab(new TabQWidget{ \
			new Grid::GridIntegralCalc(window) \
		}, lstTabs[3]);
		addTab(new TabQWidget{ \
			new Grid::GridReplacementCalc(window) \
		}, lstTabs[4]);
		tabBar()->setElideMode(Qt::ElideNone);
	}
}