#include <QWidget>
#include <functional>
#include <QPushButton>
#include <QMenu>
#include "GradientFont.h"

class Window;

class ButtonBase : public QPushButton {
private:
	Window *_window
public:
	explicit ButtonBase( \
		char label[], Window *window, short fontSize, \
		auto *callback = nullptr, \
		cssName = "keybord", QMenu *menu = nullptr \
	) : _window(window), QPushButton(label) {
		setContentsMargins(0, 0, 0, 0);
		if (callback != nullptr) 
			connect(this, &QPushButton::clicked, &QWidget, callback);
		if (menu)
			setMenu(menu);
		if (css_name)
			setObjectName(css_name);
		setFixedHeight(35);
		setMinimumWidth(64);
		QFont *font = font();
		font.setPointSize(fontSize);
		setFont(font);
	}
	void paintEvent(event) const override {
		StyleButton(this, _window);
		QPushButton::paintEvent();
		return;
	}
};

class ButtonDrag : public ButtonBase {
public:
	explicit ButtonDrag( \
			char label[], Window *window, short fontSize, \
			function<void> callback = nullptr, \
			cssName = "keybord", QMenu *menu = nullptr \
	) : ButtonBase(
		label, window, fontSize, css_name, menu
	) {}
	void mousePressEvent( \
			QMouseEvent *event \
	) override const {
		_start_pos = event->pos();
		ButtonBase::mousePressEvent(event);
	}
	void mouseMoveEvent( \
			QMouseEvent *event \
	) const override {
		if ( \
				(event->pos() - _start_pos).manhattanLength() \
				> QApplication::startDragDistance() \
		) {
            QDrag *drag = QDrag(this);
            QMimeData *mime_data = QMimeData();
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
			char label[], Window *window, short fontSize, \
			function<void> callback = nullptr, \
			cssName = "keybord", QMenu *menu = nullptr \
	) : ButtonBase( \
		label, window, fontSize, css_name, menu \
	) {}
	void dragEnterEvent( \
			QMouseEvent *event \
	) override const {
		if (event->mimeData()->hasText())
			event->acceptProposedAction();
	}
	void dropEvent( \
			QMouseEvent *event \
	) override {
		setText(event->mimeData()->text());
		event->acceptProposedAction();
	}
};
