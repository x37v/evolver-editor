/*
 *		Copyright (c) 2009 Alex Norman.  All rights reserved.
 *		http://www.x37v.info/
 *
 *		This file is part of Alex's Evolver Editor.
 *		
 *		This Evolver Editor is free software: you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License as published
 *		by the Free Software Foundation, either version 3 of the License, or (at
 *		your option) any later version.
 *		
 *		This Evolver Editor is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 *		Public License for more details.
 *		
 *		You should have received a copy of the GNU General Public License along
 *		with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

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

int SliderSpinBox::value(){
	return mSlider->value();
}

void SliderSpinBox::setValue(int val){
	mSlider->setValue(val);
}

