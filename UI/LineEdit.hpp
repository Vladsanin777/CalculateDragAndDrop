
#pragma once
#include "UI.hpp"

namespace LineEdit {
    inline LineEdit::LineEdit ( \
        Window::Window *window, byte tab, byte index, \
        const char *text \
    ) : _window{window}, QLineEdit{}, \
    _tab{tab}, _index{index} {
        setDragEnabled(true);
        std::cout << _window << std::endl;
        setText(QString::fromUtf8(text));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setSizePolicy(sizePolicy);
        connect( \
            this, &QLineEdit::textChanged, this, \
            &LineEdit::onLineEditChanged \
        );
        setMaximumHeight(40);
        setMinimumWidth(40);
        setContentsMargins(0, 0, 0, 0);
    }
    inline LineEdit::LineEdit ( \
        Window::Window *window, const char *text \
    ) : _window{window}, QLineEdit{}, \
    _tab{tab}, _index{index} {
        setDragEnabled(true);
        setText(QString::fromUtf8(text));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        setSizePolicy(sizePolicy);
        setMaximumHeight(40);
        setMinimumWidth(40);
        setContentsMargins(0, 0, 0, 0);
    }
    inline void LineEdit::focusInEvent( \
        QFocusEvent *event) {
        std::cout << (short)_tab << (short)_index << std::endl;
        _window->setInputtin(_tab, _index);
        QLineEdit::focusInEvent(event);
    }
    void LineEdit::onLineEditChanged( \
		const QString& text \
    ) const noexcept {
        // Преобразуем QString в QByteArray
        QByteArray byteArray = text.toUtf8();

        const qsizetype len { byteArray.size() };
        if (!len) { _window->setResult(strdup("0")); return; }
        // Копируем данные QByteArray в char[]
        char *textCh {new char[len + 1UL]{""}}; // +1 для нулевого завершающего символа
        strcpy(textCh, byteArray.constData());
        LogicButton::LogicCalculate *logicCalculate \
            {new LogicButton::LogicCalculate(textCh, _window)};
        if (strstr(textCh, "_ALL") != nullptr)
            logicCalculate->button_ALL();
        else if (strstr(textCh, "_O") != nullptr)
            logicCalculate->button_O();
        else if (strstr(textCh, "_RES") != nullptr)
            logicCalculate->button_RES();
        else if (strstr(textCh, "_DO"))
            logicCalculate->button_DO();
        else if (strstr(textCh, "_POS"))
            logicCalculate->button_POS();
        else
            logicCalculate->buttonOther();
    }
}