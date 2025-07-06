namespace Title {
	class TitleLayout : public QHBoxLayout {
	public:
		explicit TitleLayout( \
			Application::CalculateDragAndDrop *app, \
			Window::Window *window \
		) noexcept;
	};
	class TitleBar : public QWidget {
	private:
		TitleLayout *_child {nullptr};
	public:
		explicit TitleBar( \
			Application::CalculateDragAndDrop *app, \
			Window::Window *window \
		) noexcept;
		TitleLayout *getChild(void) noexcept;
	};
}