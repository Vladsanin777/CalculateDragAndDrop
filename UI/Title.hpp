#pragma once
#include "UI.hpp"


namespace Title {
	inline TitleLayout::TitleLayout( \
		Application::CalculateDragAndDrop *app, \
		Window::Window *window \
	) noexcept : QHBoxLayout{} {
		setContentsMargins(10, 10, 10, 10);
		setSpacing(10);
		puts("op");

		addWidget( \
			new Button::ButtonBase{ \
				"Settings", window, \
				new std::function<void(QPushButton *)> {
					std::bind(&LogicButton::visibleSetting, \
						window, std::placeholders::_1) \
				}, "basic" \
			}
		);
		Button::ButtonBase * buttonNew {
			new Button::ButtonBase{ \
				"+ New", window, \
				new std::function<void(void)> { \
					std::bind( \
						&LogicButton::createWindow, app) \
				}, "basic"}};
		buttonNew->setShortcut(QKeySequence("Ctrl+Shift+N"));
		addWidget(buttonNew);
		return;
	}
	
	inline TitleBar::TitleBar( \
		Application::CalculateDragAndDrop *app, \
		Window::Window *window \
	) noexcept : QWidget{} {
		setLayout( \
			_child = new TitleLayout(app, window) \
		);
	}
	TitleLayout *TitleBar::getChild(void) noexcept {
		return _child;
	}
}