#pragma once

#include "Label.hpp"

namespace Label {
    inline Label::Label( \
        const char * label, int fontSize \
    ) noexcept : QLabel{label} {
		setContentsMargins(0, 0, 0, 0);
        QFont m_font {font()};
        m_font.setPointSize(fontSize);
        setFont(m_font);
    }
    inline Label::Label(byte number, \
        int fontSize) noexcept : QLabel{} {

        char * label{nullptr};
        asprintf(&label, "%hhu", number);
        setText(label);
        delete [] label;
		setContentsMargins(0, 0, 0, 0);
        QFont m_font {font()};
        m_font.setPointSize(fontSize);
        setFont(m_font);
    }
}