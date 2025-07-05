#include <QPushButton>
#include "../Window/Window.cpp"

namespace Button {
	class ButtonBase;
	class ButtonDrag;
	class ButtonDragAndDrop;
	class ButtonTheme;
}

namespace Button {
	class ButtonBase : public QPushButton {
	private:
		Window::Window *_window {nullptr};
	public:
		inline explicit ButtonBase( \
			const char *label, Window::Window *window, \
			std::function<void(QPushButton *)> *callback, \
			int fontSize, const char *cssName = "basic" \
		) noexcept;
		inline explicit ButtonBase( \
			const char *label, Window::Window *window, \
			std::function<void(void)> *callback, \
			int fontSize, const char *cssName = "basic" \
		) noexcept;
		inline explicit ButtonBase( \
			const char *label, Window::Window *window, \
			int fontSize, const char *cssName = "basic" \
		) noexcept;
	};
	class ButtonDrag : public ButtonBase {
	private:
		QPoint _start_pos;
	public:
		explicit ButtonDrag( \
			const char *label, Window::Window *window, \
			std::function<void(QPushButton *)> *callback, \
			int fontSize, const char *cssName = nullptr \
		);
	protected:
		void mousePressEvent(QMouseEvent *event) override;
		void mouseMoveEvent(QMouseEvent *event) override;
	};
	class ButtonDragAndDrop : public ButtonDrag {
	public:
		explicit inline ButtonDragAndDrop( \
			const char *label, Window::Window *window, \
			std::function<void(QPushButton *)> *callback, \
			int fontSize, const char *cssName = nullptr \
		);
		static inline bool isEmpty(const char * const &text);
		static inline bool isNumber(const char * const &text);
		static inline bool isPrefix(const char * const &text);
		static inline bool isConst(const char * const &text);
		static inline bool isBracket(const char * const &text);
		static inline bool isFunction(const char * const &text);
		static inline bool is1Operator(const char * const &text);
		static inline bool is2Operator(const char * const &text);
		static inline bool is3Operator(const char * const &text);
		static inline bool is4Operator(const char * const &text);
		static inline bool isOperator(const char * const &text);
	protected:
		inline void dragEnterEvent(QDragEnterEvent *event \
		) override;
		inline void dropEvent(QDropEvent *event) override;
	};
	class ButtonTheme : public ButtonBase {
	private:
		RGB::RGB _rgb0{0, 0, 0}, _rgb1{0, 0, 0};
	public:
		inline explicit ButtonTheme(Window::Window * window, \
			byte red0, byte green0, byte blue0, \
			byte red1, byte green1, byte blue1 \
		) noexcept;
	protected:
		inline void paintEvent(QPaintEvent * event \
		) noexcept override;
	};
}