#include <QLabel>

namespace Label {
	class Label : public QLabel {
	public:
		inline explicit Label(const char * label, \
			int fontSize) noexcept;
		inline explicit Label(byte number, \
			int fontSize) noexcept;
	};
}