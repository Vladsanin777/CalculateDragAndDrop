#include <QApplication>

namespace Application {
	class CalculateDragAndDrop;
}

namespace Application {
	class CalculateDragAndDrop : public QApplication {
	public:
		inline explicit CalculateDragAndDrop( \
			int argc, char *argv[]) noexcept;
	};
}