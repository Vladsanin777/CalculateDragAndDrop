#pragma once
#include <QGridLayout>
#include <../Window/Window.cpp



namespace Window {
    class Window;
}


namespace Grid {
	template<class TButton, byte rowEnd, byte columnEnd>
	class GridCalculateKeyboard;
	class GridCalculateCommon;
	class GridBaseCalc;
	class GridDefaultCalc;
	class GridIntegralCalc;
	class GridReplacementCalc;
}


namespace Grid {
	template<class TButton, byte rowSize, byte columnSize>
	inline void buildingGridKeyboard( \
		const char * const (&buttons)[rowSize][columnSize], \
		QGridLayout *grid, Window::Window *window, \
		int fontSizeButton, \
		const byte rowStart = byte(0), \
		const byte columnStart = byte(0), \
		const char * cssName = "opertor" \
	) noexcept;
	template<class TButton, byte rowSize, byte columnSize>
	class GridCalculateKeyboard : public QGridLayout {
	public:
		inline explicit GridCalculateKeyboard( \
			const char * const (&buttons)[rowSize][columnSize], \
			Window::Window *window, int fontSize, \
			const char * cssName \
		) noexcept;
	};
	class GridCalculateCommon : public QGridLayout {
	public:
		inline explicit GridCalculateCommon( \
			Window::Window *window \
		);
	};
	class GridBaseCalc : public QGridLayout {
	public:
		inline explicit GridBaseCalc( \
			Window::Window *window, MODS mod \
		);
	};
	class GridDefaultCalc : public GridBaseCalc {
	public:
		inline explicit GridDefaultCalc( \
			Window::Window *window, MODS mod \
		);
	};
	class GridIntegralCalc : public GridBaseCalc {
	public:
		inline explicit GridIntegralCalc( \
			Window::Window *window, MODS mod = INTEGRAL \
		);
	};
	class GridReplacementCalc : public GridBaseCalc {
	public:
		inline explicit GridReplacementCalc( \
			Window::Window *window, MODS mod = REPLACEMENT \
		);
	};
}