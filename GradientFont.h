#include <QWidget>
#include <QLinearGradient>
#include <QColor>
#include <QPen>

class CreateGradient : public QLinearGradient {
public:
	explicit CreateGradient( \
		const QWidget *window, \
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
	explicit Pen(int width = 2)
	: QPen() {
		setColor(QColor("white"));
		setWidth(width);
	}
};

class StyleButton : public QPainter {
	explicit Style(QWidget *parent, QWidget *window)
	: QPainter() {
		setRenderHint(QPainter::RenderHint::Antialiasing);
		fillRect(parent.rect(), QColor("transparent"));
		setFont(parent.font());
		QFontMetrics *metrics = QFontMetrics(parent.font());
		float textX = 0.0;
		if (metrics.horizontalAdvace(parent.text()) < parent.width())
			textX = (parent.width() - \
					metrics.horizontalAdvance(parent.text())) / 2;
		QPainterPash *path = new QPainterPath();
		path.addText( \
				textX, \
				(parent.height() + metrics.height()) \
					/ 2 - metrics.descent(), \
				parent.font(), \
				parent.text() \
		);
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


class StyleLineEdit : public QPainter {

