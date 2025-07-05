namespace LogicButton {
	class LogicCalculate {
	private:
		char *_lineEditText {nullptr};
		Window::Window *_window {nullptr};
		MODS _mod {BASIC};
		byte _index{byte(0)};
	public:
        explicit LogicCalculate( \
            char *lineEditText, Window::Window *window \
        );
        inline bool isMainTab(void);
        inline bool isMainLineEdit(void);
        inline void button_ALL(void);
        inline void button_DO(void);
        inline void button_POS(void);
        inline void button_RES(void);
        inline void addHistori(void);
        inline void button_O(void);
        inline void buttonOther(void);
        inline static void inputtinLineEdit( \
            QPushButton *button, Window::Window *window \
        );
	};
	inline void visibleSetting(Window::Window * window, \
		QPushButton * button) noexcept;
	inline void changeHistoriVisible( \
		Window::Window * window, QPushButton * button \
	) noexcept;
	inline void applyLongArifmetic(Window::Window * window, \
		QPushButton* button) noexcept;
	inline void setRGB(Window::Window * window, \
		byte red0, byte green0, byte blue0, \
		byte red1, byte green1, byte blue1 \
		) noexcept;
	inline void createWindow(Application::\
		CalculateDragAndDrop * app) noexcept;
}