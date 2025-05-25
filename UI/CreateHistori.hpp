
#pragma once
#include "UI.hpp"

namespace CreateHistori {
	inline HistoriVBox::HistoriVBox( \
		const char * label, Window::Window *window \
	) : QVBoxLayout{} {
		setSpacing(0);
		setContentsMargins(0, 0, 0, 0);
		setObjectName("histori");
		addWidget(new NewHistoriElement::LabelHistori{ \
			label, "basic", window});
	}

	inline HistoriWidget::HistoriWidget( \
		const char * label, Window::Window * window \
	) : QWidget() {
		setObjectName("histori");
		setLayout(_addHistori = \
			new HistoriVBox{label, window});
		setContentsMargins(0, 0, 0, 0);
	}
	inline HistoriVBox *HistoriWidget::getAddHistori(void) {
		return _addHistori;
	}

	inline HistoriScroll::HistoriScroll( \
		const char * label, Window::Window * window \
	) : QScrollArea() {
		setHorizontalScrollBarPolicy( \
				Qt::ScrollBarPolicy::ScrollBarAlwaysOff \
		);
		setObjectName("histori");
		setWidgetResizable(true);
		setWidget(_resizeHistori = \
			new HistoriWidget{label, window});
	}
	inline HistoriWidget *HistoriScroll::getResizeHistori( \
		void) {return _resizeHistori;}
}