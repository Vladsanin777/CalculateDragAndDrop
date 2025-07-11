#include "CreateHistory.hpp"

namespace CreateHistory {
	inline HistoryVBox::HistoryVBox( \
		const char * label, Window::Window *window \
	) : QVBoxLayout{} {
		setSpacing(0);
		setContentsMargins(0, 0, 0, 0);
		setObjectName("history");
		addWidget(new NewHistoryElement::LabelHistory{ \
			label, "basic", window});
	}

	inline HistoryWidget::HistoryWidget( \
		const char * label, Window::Window * window \
	) : QWidget() {
		setObjectName("history");
		setLayout(_addHistory = \
			new HistoryVBox{label, window});
		setContentsMargins(0, 0, 0, 0);
	}
	inline HistoryVBox *HistoryWidget::getAddHistory(void) {
		return _addHistory;
	}

	inline HistoryScroll::HistoryScroll( \
		const char * label, Window::Window * window \
	) : QScrollArea() {
		setHorizontalScrollBarPolicy( \
				Qt::ScrollBarPolicy::ScrollBarAlwaysOff \
		);
		setObjectName("history");
		setWidgetResizable(true);
		setWidget(_resizeHistory = \
			new HistoryWidget{label, window});
	}
	inline HistoryWidget *HistoryScroll::getResizeHistory( \
		void) {return _resizeHistory;}
}