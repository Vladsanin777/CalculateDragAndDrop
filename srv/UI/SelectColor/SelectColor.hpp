namespace SelectColor {
	class ColorPickerArea : public QWidget {
	public:
		explicit ColorPickerArea(QWidget *parent = nullptr);
		void setHueSaturation(qreal h, qreal s);
		void setBrightness(qreal v);

	protected:
		void paintEvent(QPaintEvent *event) override;
		void mousePressEvent(QMouseEvent *event) override;
		void mouseMoveEvent(QMouseEvent *event) override;
		void resizeEvent(QResizeEvent *event) override;

	private:
		void updateBackground();
		void selectColorAt(const QPoint &pos);

		qreal hue = 0.0;
		qreal saturation = 1.0;
		qreal brightness = 1.0;
		QImage background;
		bool backgroundDirty = true;

	signals:
		void colorSelected(qreal h, qreal s);
	};
}