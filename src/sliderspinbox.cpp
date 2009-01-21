#include "sliderspinbox.hpp"
#include <QSlider>
#include <QSpinBox>
#include <QHBoxLayout>

SliderSpinBox::SliderSpinBox(QWidget * parent) : QWidget(parent){
	mSlider = new QSlider(Qt::Horizontal, this);
	mSpinBox = new QSpinBox(this);
	mLayout = new QHBoxLayout(this);

	mLayout->addWidget(mSlider, 1, Qt::AlignVCenter);
	mLayout->addWidget(mSpinBox, 0);
	mLayout->setContentsMargins(1,1,1,1);

	setLayout(mLayout);
	QObject::connect(mSlider,
			SIGNAL(valueChanged(int)),
			mSpinBox,
			SLOT(setValue(int)));
	QObject::connect(mSpinBox,
			SIGNAL(valueChanged(int)),
			mSlider,
			SLOT(setValue(int)));

	QObject::connect(mSlider,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(valueChanged(int)));
}

QSlider * SliderSpinBox::slider(){
	return mSlider;
}

QSpinBox * SliderSpinBox::spinbox(){
	return mSpinBox;
}

void SliderSpinBox::setRange(int min, int max){
	mSlider->setRange(min, max);
	mSpinBox->setRange(min, max);
}

void SliderSpinBox::setValue(int val){
	mSlider->setValue(val);
}

