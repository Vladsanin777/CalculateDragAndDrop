#include <vector>
#include <QWidget>
#include <QLinearGradient>
#include <QColor>
#include <QPen>

class Window;

class CreateGradient : public QLinearGradient {
public:
	explicit CreateGradient( \
		Window *window, \
		QWidget *widget = nullptr, \
		vector<vector<int, QColor*>> = gradient\
			{
				{0, Qt::GlobalColor::red}, 
				{1, Qt::GlobalColor::blue}
			}
	) {
		int width = window.width();
		int height = window.height();
		if (widget != nullptr) {
			QPoint *positionWidget = \
				widget.mapToGlobal(QPoint(0, 0));
			QPoint *positionWindow = \
				window.mapToGlobal(QPoint(0, 0));
			int y = positionWidget.y() - positionWindow.y();
			int x = positionWidget.x() - positionWindow.x();
			QLinearGradient(-x, -y, width / 2, height / 2);
		} else
			QLinearGradient(0, 0, width, height);
		for (colorAt : gradient) {
			setColorAt(colorAt.at(0), colorAt.at(1));
		}
	}
};

class Pen : public QPen {
public:
	explicit Pen(int width = 2)
	: QPen() {
		setColor(QColor("white"));
		setWidth(width);
	}
};

class Pash : public QPainterPash {
public:
	explicit Pash( \
			QWidget *parent, \
			int textX, int textY \
	) : QPainterPash() {
		addText(textX, textY, \
				parent->font(), parent->text() \
		);
	}
};



class StyleBase : public QPainter {
	Q_OBJECT
public:
	explicit Style( \
			QWidget *parent, Window *window, \
			Pash *path \
	) : QPainter() {
		setRenderHint(QPainter::RenderHint::Antialiasing);
		fillRect(parent.rect(), QColor("transparent"));
		setFont(parent.font());
		setPen(new Pen());
		setBrush(Qt::BrushStyle::NoBrush);
		drawPath(path);
		CreateGradient *gradient = GreateGradient( \
				parent, window \
		);
		setBrush(QBrush(gradient));
		setPen(Qt::PenStyle::NoPen);
		drawPath(path);
		end();
	}
};


class StyleLineEdit : public StyleBase {
	Q_OBJECT
public:
	explicit StyleLineEdit( \
			QWidget *parent, Window *window \
	) {
		function<int(QWidget *, QFontMetrics *)> textX = \
		[](QWidget *parent, QFontMetrics *metrics) {
			if (metrics->horizontalAdvace(parent->text()) < parent->width())
				return (parent->width() - \
					metrics.horizontalAdvance(parent->text())) / 2;
			return 0;
		}
		function<int(QWidget *, QFontMetrics *)> textY = \
		[](QWidget *parent, QFontMetrics *metrics) {
			return (parent->height() + metrics->height()) \
				/ 2 - metrics->descent();
		}
		QFontMetrics *metrics = new QFontMetrics(parent->font());
		StyleBase( \
			parent, window, \
			new Pash( \
				parent, \
				textX(parent, metrics), \
				textY(parent, metrics) \
			) \
		);
	}
};


class StyleButton : public StyleBase {
	Q_OBJECT
public:
	explicit StyleLineEdit( \
			QWidget *parent, const QWidget *window \
	) {
		function<int(QWidget *, QFontMetrics *)> textX = \
		[](QWidget *parent, QFontMetrics *metrics) {
			return parent->cursorRect()->x() - \
				metrics->horizontalAdvance( \
					parent->text()[parent->cursorPosition()] = '\0' \
				) + 5;
		}
		function<int(QWidget *, QFontMetrics *)> textY = \
		[](QWidget *parent, QFontMetrics *metrics) {
			return (parent->height() + metrics->ascent() - \
				metrics->descent()) / 2;
		}
		QFontMetrics *metrics = new QFontMetrics(parent.font());
		StyleBase( \
			parent, window, \
			new Pash( \
				parent, \
				textX(parent, metrics), \
				textY(parent, metrics) \
			) \
		);
	}
};
