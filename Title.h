#include <QHBoxLayout>
#include <Application>
#include <QWidget>

class TitleBar : public QHBoxLayout {
	Q_OBJECT
	QWidget *window
	explicit TitleBar(QApplication *app, QWidget *window) : QHBoxLayout() {
		setFixedHeight(35);
		addWidget(ButtonBase("+ Add", window, 15, app.addWindow);
		addWidget(ButtonBase("EN", window, 15, window.changeLanguage);
		addWidget(ButtonBase("Fon", window, 15, app.changeFon);
		vector<ButtonBase *> vectorButtonView = {
				ButtonBase("Basic", window, 15, localHistoriBasicVisible),
				ButtonBase("Interal", window, 15, localHistoriIntegralVisible),
				ButtonBase("Derivate", window, 15, localHistoriIntegralVisible),
				ButtonBase("Integrate", window, 15, localHistoriIntegralVisible),
				ButtonBase("Replacement", window, 15, localHistoriIntegralVisible),
			}, 
			vectorButtonLocalHistori = {
				ButtonBase("Basic", window, 15, localHistoriBasicVisible),
			};
		ButtonBase("Basic", callback=self.local_histori_basic_callback, font_size = 15, window = window, is_addition_callback = False),
		addWidget(ButtonBase(
            "View", window, 15, nullptr,
            menu=TitleMenu([
                ButtonBase("Global History", callback=self.global_histori_callback, font_size = 15, window = window, is_addition_callback = False),
                ButtonBase("Local History", menu=TitleMenu([
                    ButtonBase("Basic", callback=self.local_histori_basic_callback, font_size = 15, window = window, is_addition_callback = False),
                    ButtonBase("Integral", callback=self.local_histori_integral_callback, font_size = 15, window = window, is_addition_callback = False),
                    ButtonBase("Tab 3", font_size = 15, window = window),
                    ButtonBase("Tab 4", font_size = 15, window = window)
                ]), font_size = 15, window = window)
            ]), font_size = 15, window = window
        ))
	}
	
};
