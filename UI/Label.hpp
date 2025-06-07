#pragma once

#include "UI.hpp"

namespace Label {
    inline LabelBasic::LabelBasic( \
        const char * label, int fontSize \
    ) noexcept : QLabel{label} {
		setContentsMargins(0, 0, 0, 0);
        QFont m_font {font()};
        m_font.setPointSize(fontSize);
        setFont(m_font);
    }
}