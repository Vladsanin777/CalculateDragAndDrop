

namespace RGB {
	class RGB;
	class RGBS;
	class Slider;
	class Channel;
	class Color;
	class Gradient;
}

namespace RGB {
	class RGBA {
	private:
		byte _red{byte(0)}, _green{byte(0)}, \
			_blue{byte(0)}, _alpha{byte{0}};
		//std::function<void(byte, byte, byte)> _func;
	public:
		inline explicit RGBA(byte red, byte green, \
			byte blue, byte alpha) noexcept;
		inline void set(byte red, byte green, \
			byte blue, byte alpha) noexcept;
		
		//inline void setFunc( \
			std::function<void(byte, byte, byte)> func \
		) noexcept;
		inline byte red(void) const noexcept;
		inline byte green(void) const noexcept;
		inline byte blue(void) const noexcept;
		inline byte alpha(void) const noexcept;
	};
	class PRGBA : public RGBA {
	private:
		double _pointer {0.0};
	public:
		inline explicit PRGBA(double pointer, \
			byte red, byte green, \
			byte blue, byte alpha) noexcept;
		inline byte pointer(void) const noexcept;
	};

	class PRGBAS {
	private:
		std::vector<PRGBA> _drgbas {};
	public:
		inline explicit PRGBAS( \
			const std::vector<PRGBA> drgbas \
		) noexcept;
		PRGBA &operator[]( \
			size_t index) noexcept;
	};
	class Slider : public QSlider {
	public:
		inline explicit Slider(byte number, \
			std::function<void(void)> func, \
			Channel *parent) noexcept;
	};
	class Channel : public QBoxLayout {
	private:
		Label::Label * _label;
		Slider * _slider;
	public:
		inline explicit Channel(byte number, \
			std::function<void(void)> func) noexcept;
		inline Slider* slider(void) noexcept;
		inline Label::Label* label(void) noexcept;
	};
	class Color : public QBoxLayout {
	private:
		Channel * _redCh{nullptr}, *_greenCh{nullptr}, \
		*_blueCh{nullptr};
		Slider * _red{nullptr}, * _green{nullptr}, \
		* _blue{nullptr};
	public:
		inline explicit Color(Window::Window *window, \
			const RGB& rgb, \
			std::function<void(void)> funcApplyValue, \
			std::function<void( \
            std::function<void(byte, byte, byte)>)> func\
		) noexcept;
		inline void setValue( \
			byte red, byte green, byte blue) noexcept;
		inline byte getRedValue(void) noexcept;
		inline byte getGreenValue(void) noexcept;
		inline byte getBlueValue(void) noexcept;

	};
	class Gradient : public QGridLayout {
	private:
		Color *_first{nullptr}, *_second{nullptr};
		Window::Window * _window;
	public:
		inline explicit Gradient(Window::Window *window) noexcept;
		inline void applyGradient(void) noexcept;
	};
}