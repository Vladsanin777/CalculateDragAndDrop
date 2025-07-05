#include <QLineEdit>
#include "../Window/Window.cpp"
#include "../GeneralUI.hpp"

namespace Window {
    class Window;
}


namespace LineEdit {
	class LineEdit : public QLineEdit {
	private:
        Window::Window *_window;
        MODS _mod {BASIC};
		byte _index {byte(0)};
    public:
        explicit LineEdit ( \
            Window::Window *window, MODS mod, byte index, \
            const char *text = "" \
        );
        explicit LineEdit ( \
            Window::Window *window, const char *text = "" \
        );
    protected:
        void focusInEvent(QFocusEvent *event) override;
	private:
        void onLineEditChanged( \
            const QString& text \
        ) const noexcept;
	};
}