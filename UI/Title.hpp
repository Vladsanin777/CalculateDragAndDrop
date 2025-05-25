#pragma once
#include "UI.hpp"


namespace Title {
	inline TitleLayout::TitleLayout( \
		Application::CalculateDragAndDrop *app, \
		Window::Window *window \
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
						&Application::CalculateDragAndDrop::\
						createWindow, app, std::placeholders::_1 \
					) \
				}, "basic" \
			} \
		);
		return;
	}
	inline void TitleLayout::buttonInit(void) {
		const Window::Window * window = _window;
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
	inline void TitleLayout::changeHistoriVisible( \
		QPushButton *button) const {
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
	inline TitleBar::TitleBar( \
		Application::CalculateDragAndDrop *app, \
		Window::Window *window \
	) noexcept : QWidget() {
		setLayout( \
			_child = new TitleLayout(app, window) \
		);
	}
	TitleLayout *TitleBar::getChild(void) noexcept {
		return _child;
	}
}