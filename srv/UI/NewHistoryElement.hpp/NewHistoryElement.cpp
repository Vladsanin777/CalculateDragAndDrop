#pragma once
#include "NewHistoryElement.hpp"

namespace NewHistoriElement {
	inline LabelHistori::LabelHistori( \
		const char* label, const char *cssName, \
		Window::Window *window \
	) : QLabel(label), _callback{nullptr}, \
		_window{window} {
		setAlignment(Qt::AlignmentFlag::AlignCenter);
		setSizePolicy(QSizePolicy::Policy::Expanding, \
			QSizePolicy::Policy::Expanding);
		setContentsMargins(0, 0, 0, 0);
		setObjectName(cssName);
		this->setFont(QFont(this->font().family(), 20));
		_callback = strdup(label);
		return;
	}
	inline LabelHistori::LabelHistori( \
		const char* label, const char *cssName, \
		Window::Window *window, const char *customCallback \
	) : QLabel(label), _callback{customCallback}, \
		_window{window} {
		setAlignment(Qt::AlignmentFlag::AlignCenter);
		setSizePolicy(QSizePolicy::Policy::Expanding, \
			QSizePolicy::Policy::Expanding);
		setContentsMargins(0, 0, 0, 0);
		setObjectName(cssName);
		this->setFont(QFont(this->font().family(), 20));
		return;
	}
	inline void LabelHistori::mousePressEvent( \
		QMouseEvent* event) noexcept {
		QDrag* drag {new QDrag{this}};
		QMimeData* mimeData {new QMimeData{}};
		mimeData->setText(_callback);
		drag->setMimeData(mimeData);
		drag->exec(Qt::MoveAction);
	}
	inline LabelHistori::~LabelHistori(void) {
		delete [] _callback;
	}
	inline BaseBoxHistoriElement::\
	BaseBoxHistoriElement( \
		const char * expression, Window::Window *window, \
		const char * result
	) noexcept : QHBoxLayout() {
		setSpacing(0);
		setContentsMargins(0, 0, 0, 0);
		addWidget(new LabelHistori{expression, "keyboard", window});
		char * commonCallback {new char[ \
			strlen(expression)+strlen(result)+2UL]{'\0'}};
		strcpy(commonCallback, expression);
		strcat(commonCallback, "=");
		strcat(commonCallback, result);
		addWidget(new LabelHistori{new char[]{"="}, "keyboard", \
			window, commonCallback});
		addWidget(new LabelHistori{result, "keyboard", window});
	}
	inline BasicBoxHistoriElement:: \
	BasicBoxHistoriElement( \
		const char * expression, Window::Window * window, \
		const char * result, const char * nameOperation \
	) noexcept : QVBoxLayout() {
		setSpacing(0);
		setContentsMargins(0, 0, 0, 0);
		addWidget(new LabelHistori{nameOperation, \
			"keyboard", window});
		addLayout(new BaseBoxHistoriElement{ \
			expression, window, result});
	}
	inline SubCustomBoxHistoriElement:: \
	SubCustomBoxHistoriElement( \
		Window::Window * window, \
		const char * label1, const char * text1, \
		const char * label2, const char * text2 \
	) noexcept : QHBoxLayout() {
		setSpacing(0);
		setContentsMargins(0, 0, 0, 0);
		addWidget(new LabelHistori{ \
			label1, "keybord", window});
		char *customCallback{new char[ \
			strlen(label1) + strlen(text1) + 2UL]{'\0'}};
		strcpy(customCallback, label1);
		strcat(customCallback, "=");
		strcat(customCallback, text1);
		addWidget(new LabelHistori{ \
			new char[]{"="}, "keybord", \
			window, customCallback});
		addWidget(new LabelHistori{ \
			text1, "keyboard", window});

		addWidget(new LabelHistori{ \
			label2, "keyboard", window});
		customCallback = new char[ \
			strlen(label1) + strlen(text1) + 2UL]{'\0'};
		strcpy(customCallback, label2);
		strcat(customCallback, "=");
		strcat(customCallback, text2);
		addWidget(new LabelHistori{ \
			new char[]{"="}, "keyboard", \
			window, customCallback});
		addWidget(new LabelHistori{ \
			text2, "keyboard", window});
	}
	inline CustomBoxHistoriElement:: \
	CustomBoxHistoriElement( \
		const char * const expression, \
		const char * const result, \
		Window::Window * const  window, byte tab, \
		const char * const label1, \
		const char * const text1, \
		const char * const label2, \
		const char * const text2 \
	) noexcept : QVBoxLayout() {
		setSpacing(0);
		setContentsMargins(0, 0, 0, 0);
		addLayout(new SubCustomBoxHistoriElement{ \
			window, label1, text1, \
			label2, text2});
		addLayout(new BaseBoxHistoriElement{ \
			expression, window, result});
	}
	inline CustomBoxHistoriElement:: \
	CustomBoxHistoriElement( \
		const char * const expression, \
		const char * const result, \
		Window::Window * const  window, byte tab, \
		const char * const label1, \
		const char * const text1, \
		const char * const label2, \
		const char * const text2, \
		const char * const  nameOperation \
	) noexcept : QVBoxLayout() {
		setSpacing(0);
		setContentsMargins(0, 0, 0, 0);
		addWidget(new LabelHistori{ \
			nameOperation, "keyboard", window});
		addLayout(new SubCustomBoxHistoriElement{ \
			window, label1, text1, \
			label2, text2});
		addLayout(new BaseBoxHistoriElement{ \
			expression, window, result});
	}
}