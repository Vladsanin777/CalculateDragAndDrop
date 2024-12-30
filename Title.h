#include <vector>
#include <QApplication>
#include <QHBoxLayout>
#include <QWidget>
#include <QMenu>
#include <QScrollArea>
#include <QWidgetAction>
#include "Button.h"

class Window;

class Action : public QWidgetAction {
    Q_OBJECT
public:
    explicit Action(Menu *parent, ButtonBase *button)
        : QWidgetAction(parent) {
        setDefaultWidget(button); // Add semicolon here
    }
};
class Menu : public QMenu {
	Q_OBJECT
private:
	explicit Menu( \
			vector<ButtonBase *> buttons \
	) : QMenu() {
		setAttribute( \
				Qt.WidgetAttribute.WA_TranslucentBackground \
		);
		for (button : buttons) {
			addAction(Action(this, button))
		}
	}
};


class TitleBar : public QHBoxLayout {
	Q_OBJECT
private:
	QWidget    *_window                 = nullptr;
	QScrollArea *_globalHistori          = nullptr;
	QScrollArea *_localHistoriBasic      = nullptr;
	QScrollArea *_localHistoriIntegral   = nullptr;
	QScrollArea *_localHistoriDerivative = nullptr;
	QScrollArea *_localHistoriIntegrate  = nullptr;
	QScrollArea *_localHistoriReplacement = nullptr;
public:
	explicit TitleBar( \
			const QApplication *app, Window *window \
	) : window(window), QHBoxLayout() {
		setFixedHeight(35);
		addWidget(ButtonBase("+ Add", window, 15, &app::addWindow);
		addWidget(ButtonBase("EN",    window, 15, &window::changeLanguage);
		addWidget(ButtonBase("Fon",   window, 15, &app::changeFon);
		vector<ButtonBase *> vectorButtonLocalHistori = {
			new ButtonBase("Basic",       window, 15, \
					&TitleBar::localHistoriBasicVisible \
			),
			new ButtonBase("Integral",    window, 15, \
					&TitleBar::localHistoriIntegralVisible \
			),
			new ButtonBase("Derivative",  window, 15, \
					&TitleBar::localHistoriDerivativeVisible \
			),
			new ButtonBase("Integrate",   window, 15, \
					&TitleBar::localHistoriIntegrateVisible \
			),
			new ButtonBase("Replacement", window, 15, \
					&TitleBar::localHistoriReplacementVisible \
			),
		}, vectorButtonView = {
			new ButtonBase("Global Histori", window, 15, \
					&TitleBar::globalHistoriVisible),
			new ButtonBase("Local Histori",  window, 15, \
					nullptr, "keybord", Menu(vectorButtonLocalHistori))
		};
		addWidget(ButtonBase("Basic", window, 15, \
				nullptr, "keybord", Menu(vectorButtonView)));
		_globalHistori           = window->getGlobalHistori();
		_localHistoriBasic       = window->getLocalHistori(0);
		_localHistoriIntegral    = window->getLocalHistori(1);
		_localHistoriDerivative  = window->getLocalHistori(2);
		_localHistoriIntegrate   = window->getLocalHistori(3);
		_localHistoriReplacement = window->getLocalHistori(4);
		return;
	}
	void globalHistoriVisible() const final {
		_globalHistori->setVisible( \
				!_globalHistori->isVisible() \
		);
		return;
	}
	void localHistoriBasicVisible() const final {
		_localHistoriBasic->setVisible( \
				!_localHistoriBasic->isVisible() \
		);
		return;
	}
	void localHistoriIntegralVisible() const final {
		_localHistoriIntegral->setVisible( \
				!_localHistoriIntegral->isVisible() \
		);
		return;
	}
	void localHistoriDerivativeVisible() const final {
		_localHistoriDerivative->setVisible( \
				!_localHistoriDerivative->isVisible() \
		);
		return;
	}
	void localHistoriIntegrateVisible() const final {
		_localHistoriIntegrate->setVisible( \
				!_localHistoriIntegrate->isVisible()
		);
		return;
	}
	void localHistoriReplacementVisible() const final {
		_localHistoriReplacement->setVisible( \
				!_localHistoriReplacement->isVisible()
		);
		return;
	}
};
