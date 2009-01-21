#ifndef SLIDER_SPIN_BOX_HPP
#define SLIDER_SPIN_BOX_HPP

#include <QWidget>
class QSlider;
class QSpinBox;
class QHBoxLayout;

//this is a slider with a spinbox under it
class SliderSpinBox : public QWidget {
	Q_OBJECT
	public:
		SliderSpinBox(QWidget * parent = NULL);
		QSlider * slider();
		QSpinBox * spinbox();
		void setRange(int min, int max);
	public slots:
		void setValue(int val);
	signals:
		void valueChanged(int);
	protected:
		QSlider * mSlider;
		QSpinBox * mSpinBox;
		QHBoxLayout * mLayout;
};

#endif
