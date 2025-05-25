
#pragma once
#include "UI.hpp"

namespace Button {
	inline ButtonBase::ButtonBase( \
		const char *label, Window::Window *window, \
		std::function<void(QPushButton *)> \
		*callback, const char *cssName \
	) : _window(window), QPushButton(label) {
		setAttribute(Qt::WA_Hover, true);
		setContentsMargins(0, 0, 0, 0);
		if (callback) 
			connect(this, &QPushButton::clicked, \
				[this, callback](bool) {
				if (callback) {
					(*callback)(this);
				}
			});
		if (cssName)
			setObjectName(cssName);
		setMinimumWidth(70);
		setMinimumHeight(40);
	}

	inline ButtonDrag::ButtonDrag( \
		const char *label, Window::Window *window, \
		std::function<void(QPushButton *)> \
		*callback, const char *cssName \
	) : ButtonBase(label, window, callback, cssName) {}
	inline void ButtonDrag::mousePressEvent( \
		QMouseEvent *event) {
		QPushButton::mousePressEvent(event);
		_start_pos = event->pos();
	}
	inline void ButtonDrag::mouseMoveEvent(QMouseEvent *event) {
		// 1. Сначала всегда вызываем базовую обработку (для hover)
		QPushButton::mouseMoveEvent(event);

		// 2. Проверяем, что зажата именно левая кнопка мыши
		if (!(event->buttons() & Qt::LeftButton))
			return;

		// 3. Увеличиваем порог активации DnD (в 2-3 раза)
		const int dragThreshold = QApplication::startDragDistance() * 2;
		
		// 4. Проверяем расстояние от точки нажатия
		if ((event->pos() - _start_pos).manhattanLength() > dragThreshold) {
			// 5. Создаем DnD операцию
			QDrag* drag = new QDrag(this);
			QMimeData* mimeData = new QMimeData();
			mimeData->setText(text());
			drag->setMimeData(mimeData);
			
			// 6. Запускаем DnD и сохраняем результат
			Qt::DropAction result = drag->exec(Qt::MoveAction);
			
			// 7. После DnD принудительно обновляем состояние кнопки
			if (result != Qt::IgnoreAction) {
				setDown(false);  // Снимаем "нажатое" состояние
				update();        // Перерисовываем кнопку
			}
		}
	}

	inline ButtonDragAndDrop:: \
	ButtonDragAndDrop( \
		const char *label, Window::Window *window, \
		std::function<void(QPushButton *)> \
		*callback, const char *cssName \
	) : ButtonDrag{label, window, callback, cssName} \
	{setAcceptDrops(true);}
	
	inline bool ButtonDragAndDrop::isEmpty( \
		const char * const &text) {
		return strlen(text) == 0UL;
	}

	inline bool ButtonDragAndDrop::isNumber( \
		const char * const &text) {
		size_t len {strlen(text)};
		if (!(isdigit(*text) || ('-' == *text \
			&& len > 1UL)))
			return false;
		if (text[len-1UL] == '%') len--;
		for (const char * ptr {text+1}; --len ; ptr++)
			if (!isdigit(*ptr) && \
				*ptr != '.' && *ptr != ',')
				return false;
		return true;
	}
	inline bool ButtonDragAndDrop::isPrefix( \
		const char * const &text) {
		size_t len {strlen(text)};
		if (len < 2) return false;
		if (*text != '0') return false;
		const char second {text[1]};
		if (second != 'b' && \
			second != 'x' && second != 'o')
			return false;
		if (len == 2) return true;
		const char *textlvalue {text+2UL};
		return isNumber(textlvalue);
	}
	inline bool ButtonDragAndDrop::isConst( \
		const char * const &text) {
		return strcmp(text, "_E") == 0 || \
			strcmp(text, "_PI") == 0;
	}
	inline bool ButtonDragAndDrop::isBracket( \
		const char * const &text) {
		for (const char * ptr {text}, \
			* const ptrEnd {text+strlen(text)}; \
			ptr != ptrEnd; ptr++) {
			switch (*ptr) {
				case '(':
				case ')':
				case '[':
				case ']':
				case '{':
				case '}':
					continue;
				default:
					return false;
			}
		}
		return true;
	}
	inline bool ButtonDragAndDrop::isFunction( \
		const char * const &text) {
		return strcmp(text, "_ALL") == 0 || \
			strcmp(text, "_DO") == 0 || \
			strcmp(text, "_RES")  == 0 || \
			strcmp(text, "_POS") == 0 || \
			strcmp(text, "_O") == 0;
	}
	inline bool ButtonDragAndDrop::is1Operator( \
		const char * const &text) {
		if (strlen(text) != 1) return false;
		char fristSymbol {*text};
		switch (fristSymbol) {
			case '+':
			case '-':
			case ':':
			case '*':
			case '^':
			case '!':
				return true;
			default:
				return false;
		}
	}
	inline bool ButtonDragAndDrop::is2Operator( \
		const char * const &text) {
		return *text == 'l' && \
			(text[1] == 'n' || text[1] == 'g') && \
			(text[2] == '\0' || isBracket(text));
	}
	inline bool ButtonDragAndDrop::is3Operator( \
		const char * const &text) {
		if (strcmp(text, "mod") == 0) return true;
		if (memcmp(text, "log", 3UL) == 0 || \
			memcmp(text, "sin", 3UL) == 0 || \
			memcmp(text, "cos", 3UL) == 0 || \
			memcmp(text, "tan", 3UL) == 0 || \
			memcmp(text, "sec", 3UL) == 0 || \
			memcmp(text, "csc", 3UL) == 0 || \
			memcmp(text, "cot", 3UL) == 0 || \
			memcmp(text, "sgn", 3UL) == 0 || \
			memcmp(text, "abs", 3UL) == 0 || \
			memcmp(text, "exp", 3UL) == 0 || \
			memcmp(text, "sqr", 3UL) == 0 \
		) {
			const char *textlvalue {text+3UL};
			return isBracket(textlvalue);
		}
		return false;
	}
	inline bool ButtonDragAndDrop::is4Operator( \
		const char * const &text) {
		if (memcmp(text, "sqrt", 4UL) == 0 || \
			memcmp(text, "asin", 4UL) == 0 || \
			memcmp(text, "acos", 4UL) == 0 || \
			memcmp(text, "atan", 4UL) == 0 || \
			memcmp(text, "asec", 4UL) == 0 || \
			memcmp(text, "acsc", 4UL) == 0 || \
			memcmp(text, "acot", 4UL) == 0 || \
			memcmp(text, "cbrt", 4UL) == 0 \
		) {
			const char *textlvalue {text+4UL};
			return isBracket(text);
		}
		return false;
	}
	inline bool ButtonDragAndDrop::isOperator( \
		const char * const &text) {
		if (is1Operator(text)) return true;
		if (is2Operator(text)) return true;
		if (is3Operator(text)) return true;
		if (is4Operator(text)) return true;
		return false;
	}
	inline void ButtonDragAndDrop::dragEnterEvent( \
			QDragEnterEvent *event) {
		QPushButton::dragEnterEvent(event);
		if (event->mimeData()->hasText())
			event->acceptProposedAction();
		return;
	}
	inline void ButtonDragAndDrop::dropEvent( \
		QDropEvent *event) {
		QPushButton::dropEvent(event);
		const char * text { \
			event->mimeData()->text().toUtf8().constData()}, \
			* cssName {nullptr};
		setText(text);
		if (isEmpty(text)) cssName = "empty";
		else if (isNumber(text)) cssName = "number";
		else if (isPrefix(text)) cssName = "prefix";
		else if (isBracket(text)) cssName = "bracket";
		else if (isConst(text)) cssName = "const";
		else if (isFunction(text)) cssName = "function";
		else if (isOperator(text)) cssName = "operator";
		else cssName = "expression";
		puts(cssName);
		setStyleSheet("");
		setObjectName(cssName);
		style()->unpolish(this);
		style()->polish(this);
		update();
		event->acceptProposedAction();
		return;
	}
}