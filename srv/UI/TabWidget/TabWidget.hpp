namespace TabWidget {
	class TabQWidget : public QWidget {
	public:
		inline explicit TabQWidget(QGridLayout *tab);
	};

	class TabWidgetKeyboard : public QTabWidget {
	public:
		inline explicit TabWidgetKeyboard(Window::Window *window);
	};

	class MainTabWidget : public QTabWidget {
	public:
		inline explicit MainTabWidget(Window::Window *window);
	};
}