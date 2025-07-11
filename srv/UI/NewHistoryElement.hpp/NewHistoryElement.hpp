#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "../Window/Window.cpp"

namespace Window {
	class Window;
}

class QLabel;
class QHBoxLayout;
class QVBoxLayout;

namespace NewHistoryElement {
	class LabelHistory;
	class BaseBoxHistoryElement;
	class BasicBoxHistoryElement;
	class SubCustomBoxHistoryElement;
	class CustomBoxHistoryElement;
}

namespace NewHistoryElement {
	class LabelHistory : public QLabel {
	private:
		Window::Window *_window {nullptr};
		const char *_callback {nullptr};
	public:
		explicit inline LabelHistory( \
			const char* label, const char *cssName, \
			Window::Window *window \
		);
		explicit inline LabelHistory( \
			const char* label, const char *cssName, \
			Window::Window *window, const char *customCallback \
		);
	protected:
		inline void mousePressEvent(QMouseEvent* event \
		) noexcept override;
	private:
		~LabelHistory(void);
	};
	class BaseBoxHistoryElement : public QHBoxLayout {
	public:
		explicit inline BaseBoxHistoryElement( \
			const char * expression, Window::Window *window, \
			const char * result
		) noexcept;
	};
	class BasicBoxHistoryElement : public QVBoxLayout {
	public:
		explicit inline BasicBoxHistoryElement( \
			const char * expression, Window::Window * window, \
			const char * result, const char * nameOperation \
		) noexcept;
	};
	class SubCustomBoxHistoryElement : public QHBoxLayout {
	public:
		explicit inline SubCustomBoxHistoryElement( \
			Window::Window * window, \
			const char * label1, const char * text1, \
			const char * label2, const char * text2 \
		) noexcept;
	};
	class CustomBoxHistoryElement : public QVBoxLayout {
	public:
		explicit inline CustomBoxHistoryElement( \
			const char * const expression, \
			const char * const result, \
			Window::Window * const  window, byte tab, \
			const char * const label1, \
			const char * const text1, \
			const char * const label2, \
			const char * const text2 \
		) noexcept;
		explicit inline CustomBoxHistoryElement( \
			const char * const expression, \
			const char * const result, \
			Window::Window * const  window, byte tab, \
			const char * const label1, \
			const char * const text1, \
			const char * const label2, \
			const char * const text2, \
			const char * const  nameOperation \
		) noexcept;
	};
}