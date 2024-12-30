#include <QLineEdit>
#include <QFocusEvent>
#include <cstring>
#include "GradientFont.h"

class Window;

class  LineEdit : public QLineEdit {
private:
	Window *_window;
	const short _inputtin;
public:
	explicit LineEdit ( \
			Window *window, \
			int *inputtin[2], char *text[] \
	) : _window(window), _inputtin(inputtin), \
	QLineEdit() {
		setText(text);
		QSizePolicy *sizePolicyExpanding = nullptr;
		setSizePolicy( \
				sizePolicyExpanding = \
					sizePolicy().Policy.Expanding, \
				sizePolicyExpanding \
		);
		setObjectName("keybord");
		textChanged.connect(onLineEditChanged);
		const QFont font = this->font();
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
			_window->setInputtin(_inputtin);
		QLineEdit::focusInEvent(event);
	}
	void onLineEditChanged( \
			char textLineEdit[] \
	) const {
		LogicCalculate *logicCalculate = \
			new LogicCalculate(textLineEdit, _window);
		if (strstr(textLineEdit, "_ALL") != nullptr)
			logicCalculate.button_ALL();
		else if (strstr(textLineEdit, "_O") != nullptr)
			logicCalculate.button_O();
		else if (strstr(textLineEdit, "_RES") != nullptr)
			logicCalculate.button_RES();
		else
			logicCalculate.buttonOther();
	}
	void paintEvent( \
			QPaintEvent *event \
	) override {
		StyleLineEdit(this, _window);
		QLineEdit::paintEvent(event);
	}
};
