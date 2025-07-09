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

namespace NewHistoriElement {
	class LabelHistori : public QLabel {
	private:
		Window::Window *_window {nullptr};
		const char *_callback {nullptr};
	public:
		explicit inline LabelHistori( \
			const char* label, const char *cssName, \
			Window::Window *window \
		);
		explicit inline LabelHistori( \
			const char* label, const char *cssName, \
			Window::Window *window, const char *customCallback \
		);
	protected:
		inline void mousePressEvent(QMouseEvent* event \
		) noexcept override;
	private:
		~LabelHistori(void);
	};
	class BaseBoxHistoriElement : public QHBoxLayout {
	public:
		explicit inline BaseBoxHistoriElement( \
			const char * expression, Window::Window *window, \
			const char * result
		) noexcept;
	};
	class BasicBoxHistoriElement : public QVBoxLayout {
	public:
		explicit inline BasicBoxHistoriElement( \
			const char * expression, Window::Window * window, \
			const char * result, const char * nameOperation \
		) noexcept;
	};
	class SubCustomBoxHistoriElement : public QHBoxLayout {
	public:
		explicit inline SubCustomBoxHistoriElement( \
			Window::Window * window, \
			const char * label1, const char * text1, \
			const char * label2, const char * text2 \
		) noexcept;
	};
	class CustomBoxHistoriElement : public QVBoxLayout {
	public:
		explicit inline CustomBoxHistoriElement( \
			const char * const expression, \
			const char * const result, \
			Window::Window * const  window, byte tab, \
			const char * const label1, \
			const char * const text1, \
			const char * const label2, \
			const char * const text2 \
		) noexcept;
		explicit inline CustomBoxHistoriElement( \
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