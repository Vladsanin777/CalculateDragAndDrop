#include <QWidget>
#include <functional>
#include <QPushButton>
#include <QMenu>

class ButtonBase : public QPushButton {
private:
	QWidget *_window
public:
	explicit ButtonBase( \
		char label[], QWidget *window, short fontSize, \
		function<void> callback = nullptr, \
		cssName = "keybord", QMenu *menu = nullptr \
	) : window(window), QPushButton(label) {
		setContentsMargin(0, 0, 0, 0);
		if (callback) 
			clicked->connect(callback);
		if (menu)
			setMenu(menu);
		if (css_name)
			setObjectName(css_name);
		setfixedHeight(35);
		setMinimumWidth(64);
		QFont *font = font();
		font.setPointSize(fontSize);
		setFont(font);
	}
	void paintEvent(event) const override final {
		StyleButton(this, window);
		QPushButton::paintEvent();
		return;
	}
};

class ButtonDrag : public ButtonBase {
public:
	explicit ButtonDrag( \
			char label[], QWidget *window, short fontSize, \
			function<void> callback = nullptr, \
			cssName = "keybord", QMenu *menu = nullptr \
	) : window(window), ButtonBase(
		label, window, fontSize, css_name, menu
	) {}
	void mousePressEvent( \
			QMouseEvent *event \
	) override const final {
		_start_pos = event->pos();
		ButtonBase::mousePressEvent(event);
	}
	void mouseMoveEvent( \
			QMouseEvent *event \
	) const final override {
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

class ButtonDragAndDrop : public ButtonDrag final {
	explicit ButtonDragAndDrop( \
			char label[], QWidget *window, short fontSize, \
			function<void> callback = nullptr, \
			cssName = "keybord", QMenu *menu = nullptr \
	) : window(window), ButtonBase( \
		label, window, fontSize, css_name, menu \
	) {}
	void dragEnterEvent( \
			QMouseEvent *event \
	) override final const {
		if (event->mimeData()->hasText())
			event->acceptProposedAction();
	}
	void dropEvent( \
			QMouseEvent *event \
	) override final {
		setText(event->mimeData()->text());
		event->acceptProposedAction();
	}
};
