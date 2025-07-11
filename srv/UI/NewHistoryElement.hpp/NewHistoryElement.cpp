#pragma once
#include "NewHistoryElement.hpp"

namespace NewHistoryElement {
	inline LabelHistory::LabelHistory( \
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
	inline LabelHistory::LabelHistory( \
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
	inline void LabelHistory::mousePressEvent( \
		QMouseEvent* event) noexcept {
		QDrag* drag {new QDrag{this}};
		QMimeData* mimeData {new QMimeData{}};
		mimeData->setText(_callback);
		drag->setMimeData(mimeData);
		drag->exec(Qt::MoveAction);
	}
	inline LabelHistory::~LabelHistory(void) {
		delete [] _callback;
	}
	inline BaseBoxHistoryElement::\
	BaseBoxHistoryElement( \
		const char * expression, Window::Window *window, \
		const char * result
	) noexcept : QHBoxLayout() {
		setSpacing(0);
		setContentsMargins(0, 0, 0, 0);
		addWidget(new LabelHistory{expression, "keyboard", window});
		char * commonCallback {new char[ \
			strlen(expression)+strlen(result)+2UL]{'\0'}};
		strcpy(commonCallback, expression);
		strcat(commonCallback, "=");
		strcat(commonCallback, result);
		addWidget(new LabelHistory{new char[]{"="}, "keyboard", \
			window, commonCallback});
		addWidget(new LabelHistory{result, "keyboard", window});
	}
	inline BasicBoxHistoryElement:: \
	BasicBoxHistoryElement( \
		const char * expression, Window::Window * window, \
		const char * result, const char * nameOperation \
	) noexcept : QVBoxLayout() {
		setSpacing(0);
		setContentsMargins(0, 0, 0, 0);
		addWidget(new LabelHistory{nameOperation, \
			"keyboard", window});
		addLayout(new BaseBoxHistoryElement{ \
			expression, window, result});
	}
	inline SubCustomBoxHistoryElement:: \
	SubCustomBoxHistoryElement( \
		Window::Window * window, \
		const char * label1, const char * text1, \
		const char * label2, const char * text2 \
	) noexcept : QHBoxLayout() {
		setSpacing(0);
		setContentsMargins(0, 0, 0, 0);
		addWidget(new LabelHistory{ \
			label1, "keybord", window});
		char *customCallback{new char[ \
			strlen(label1) + strlen(text1) + 2UL]{'\0'}};
		strcpy(customCallback, label1);
		strcat(customCallback, "=");
		strcat(customCallback, text1);
		addWidget(new LabelHistory{ \
			new char[]{"="}, "keybord", \
			window, customCallback});
		addWidget(new LabelHistory{ \
			text1, "keyboard", window});

		addWidget(new LabelHistory{ \
			label2, "keyboard", window});
		customCallback = new char[ \
			strlen(label1) + strlen(text1) + 2UL]{'\0'};
		strcpy(customCallback, label2);
		strcat(customCallback, "=");
		strcat(customCallback, text2);
		addWidget(new LabelHistory{ \
			new char[]{"="}, "keyboard", \
			window, customCallback});
		addWidget(new LabelHistory{ \
			text2, "keyboard", window});
	}
	inline CustomBoxHistoryElement:: \
	CustomBoxHistoryElement( \
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
		addLayout(new SubCustomBoxHistoryElement{ \
			window, label1, text1, \
			label2, text2});
		addLayout(new BaseBoxHistoryElement{ \
			expression, window, result});
	}
	inline CustomBoxHistoryElement:: \
	CustomBoxHistoryElement( \
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
		addWidget(new LabelHistory{ \
			nameOperation, "keyboard", window});
		addLayout(new SubCustomBoxHistoryElement{ \
			window, label1, text1, \
			label2, text2});
		addLayout(new BaseBoxHistoryElement{ \
			expression, window, result});
	}
}