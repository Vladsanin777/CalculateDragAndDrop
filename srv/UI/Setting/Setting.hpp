namespace Setting {
	class SettingGrid : public QGridLayout {
	public:
		inline explicit SettingGrid(Window::Window * window, \
			QWidget * parent = nullptr) noexcept;

	};
	class SettingWidget : public QWidget {
	public:
		inline explicit SettingWidget(Window::Window * window, \
			QWidget * parent = nullptr) noexcept;

	};
	class SettingArea : public QScrollArea {
	public:
		inline explicit SettingArea(Window::Window * window, \
		QWidget * parent = nullptr) noexcept;
		void setWidget(QWidget *widget) noexcept;
		inline bool eventFilter(QObject *obj, \
			QEvent *event) noexcept override;

	protected:
		inline void resizeEvent(QResizeEvent *event \
		) noexcept override;

	private:
		inline void updateMinimumWidth(void) noexcept;
	};
	class SettingDock : public QDockWidget {
	private:
		const RGB::RGB& _rgb0, &_rgb1;
	public:
		inline explicit SettingDock(Window::Window * window \
		) noexcept;
	protected:
		inline void paintEvent(QPaintEvent *event) override;
	};
}