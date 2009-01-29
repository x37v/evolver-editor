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

#include "vca.hpp"
#include "common.hpp"

const unsigned int VCAModel::level_max = 100;
const unsigned int VCAModel::env_amount_max = 100;
const unsigned int VCAModel::attack_max = 110;
const unsigned int VCAModel::decay_max = 110;
const unsigned int VCAModel::sustain_max = 100;
const unsigned int VCAModel::release_max = 110;
const unsigned int VCAModel::velocity_max = 100;
const char * VCAModel::pan_type_names[] = {
	"stereo hard pan",
	"stereo some center",
	"stereo more center",
	"mono",
	"rev stereo more center",
	"rev stereo some center",
	"rev stereo hard pan"
};
const unsigned int VCAModel::num_pan_types = 7;

VCAModel::VCAModel(QObject * parent) : Model(parent){
	mLevel = 0;
	mEnvAmount = 0;
	mAttack = 0;
	mDecay = 0;
	mSustain = 0;
	mRelease = 0;
	mVelocity = 0;
	mPanType = stereo_one;
}

void VCAModel::set_level(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mLevel, level_max)){
		mLevel = val;
		emit(level_changed(mLevel));
		send_program_param(24, mLevel);
	}
}

void VCAModel::set_env_amount(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mEnvAmount, env_amount_max)){
		mEnvAmount = val;
		emit(env_amount_changed(mEnvAmount));
		send_program_param(25, mEnvAmount);
	}
}

void VCAModel::set_attack(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mAttack, attack_max)){
		mAttack = val;
		emit(attack_changed(mAttack));
		send_program_param(26, mAttack);
	}
}

void VCAModel::set_decay(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mDecay, decay_max)){
		mDecay = val;
		emit(decay_changed(mDecay));
		send_program_param(27, mDecay);
	}
}

void VCAModel::set_sustain(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mSustain, sustain_max)){
		mSustain = val;
		emit(sustain_changed(mSustain));
		send_program_param(28, mSustain);
	}
}

void VCAModel::set_release(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mRelease, release_max)){
		mRelease = val;
		emit(release_changed(mRelease));
		send_program_param(29, mRelease);
	}
}

void VCAModel::set_velocity(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mVelocity, velocity_max)){
		mVelocity = val;
		emit(velocity_changed(mVelocity));
		send_program_param(89, mVelocity);
	}
}

void VCAModel::set_pan(int val){
	if(val < 0)
		return;
	pan_type pan = (pan_type)val;
	if(pan != mPanType){
		mPanType = pan;
		emit(pan_changed(mPanType));
		send_program_param(30, mPanType);
	}
}


#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include "sliderspinbox.hpp"

VCAView::VCAView(QWidget * parent) : QWidget(parent){
	QLabel * lab;
	//allocate
	mLayout = new QGridLayout(this);

	mLevel = new SliderSpinBox(this);
	mEnvAmount = new SliderSpinBox(this);
	mAttack = new SliderSpinBox(this);
	mDecay = new SliderSpinBox(this);
	mSustain = new SliderSpinBox(this);
	mRelease = new SliderSpinBox(this);
	mVelocity = new SliderSpinBox(this);
	mPanType = new QComboBox(this);

	//set up
	mLevel->setRange(0, VCAModel::level_max);
	mEnvAmount->setRange(0, VCAModel::env_amount_max);
	mAttack->setRange(0, VCAModel::attack_max);
	mDecay->setRange(0, VCAModel::decay_max);
	mSustain->setRange(0, VCAModel::sustain_max);
	mRelease->setRange(0, VCAModel::release_max);
	mVelocity->setRange(0, VCAModel::velocity_max);
	for(unsigned int i = 0; i < VCAModel::num_pan_types; i++)
		mPanType->addItem(QString(VCAModel::pan_type_names[i]));

	//label and plot
	lab = new QLabel(QString("level"));
	mLayout->addWidget(mLevel, 0, 1);
	mLayout->addWidget(lab, 0, 0, Qt::AlignRight);

	lab = new QLabel(QString("env amount"));
	mLayout->addWidget(mEnvAmount, 1, 1);
	mLayout->addWidget(lab, 1, 0, Qt::AlignRight);

	lab = new QLabel(QString("attack"));
	mLayout->addWidget(mAttack, 2, 1);
	mLayout->addWidget(lab, 2, 0, Qt::AlignRight);

	lab = new QLabel(QString("decay"));
	mLayout->addWidget(mDecay, 3, 1);
	mLayout->addWidget(lab, 3, 0, Qt::AlignRight);

	lab = new QLabel(QString("sustain"));
	mLayout->addWidget(mSustain, 4, 1);
	mLayout->addWidget(lab, 4, 0, Qt::AlignRight);

	lab = new QLabel(QString("release"));
	mLayout->addWidget(mRelease, 5, 1);
	mLayout->addWidget(lab, 5, 0, Qt::AlignRight);

	lab = new QLabel(QString("velocity"));
	mLayout->addWidget(mVelocity, 6, 1);
	mLayout->addWidget(lab, 6, 0, Qt::AlignRight);

	lab = new QLabel(QString("pan"));
	mLayout->addWidget(mPanType, 7, 1);
	mLayout->addWidget(lab, 7, 0, Qt::AlignRight);

	mLayout->setSpacing(2);
	mLayout->setRowStretch(10, 1);

	setLayout(mLayout);

	//connect out signals
	QObject::connect(mLevel,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(level_changed(int)));
	QObject::connect(mEnvAmount,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(env_amount_changed(int)));
	QObject::connect(mAttack,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(attack_changed(int)));
	QObject::connect(mDecay,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(decay_changed(int)));
	QObject::connect(mSustain,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(sustain_changed(int)));
	QObject::connect(mRelease,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(release_changed(int)));
	QObject::connect(mVelocity,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(velocity_changed(int)));
	QObject::connect(mPanType,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(pan_changed(int)));
}

void VCAView::connect_to_model(VCAModel * model){

	QObject::connect(
			this,
			SIGNAL(level_changed(int)),
			model,
			SLOT(set_level(int)));
	QObject::connect(
			model,
			SIGNAL(level_changed(int)),
			this,
			SLOT(set_level(int)));

	QObject::connect(
			this,
			SIGNAL(env_amount_changed(int)),
			model,
			SLOT(set_env_amount(int)));
	QObject::connect(
			model,
			SIGNAL(env_amount_changed(int)),
			this,
			SLOT(set_env_amount(int)));

	QObject::connect(
			this,
			SIGNAL(attack_changed(int)),
			model,
			SLOT(set_attack(int)));
	QObject::connect(
			model,
			SIGNAL(attack_changed(int)),
			this,
			SLOT(set_attack(int)));

	QObject::connect(
			this,
			SIGNAL(decay_changed(int)),
			model,
			SLOT(set_decay(int)));
	QObject::connect(
			model,
			SIGNAL(decay_changed(int)),
			this,
			SLOT(set_decay(int)));

	QObject::connect(
			this,
			SIGNAL(sustain_changed(int)),
			model,
			SLOT(set_sustain(int)));
	QObject::connect(
			model,
			SIGNAL(sustain_changed(int)),
			this,
			SLOT(set_sustain(int)));

	QObject::connect(
			this,
			SIGNAL(release_changed(int)),
			model,
			SLOT(set_release(int)));
	QObject::connect(
			model,
			SIGNAL(release_changed(int)),
			this,
			SLOT(set_release(int)));

	QObject::connect(
			this,
			SIGNAL(velocity_changed(int)),
			model,
			SLOT(set_velocity(int)));
	QObject::connect(
			model,
			SIGNAL(velocity_changed(int)),
			this,
			SLOT(set_velocity(int)));

	QObject::connect(
			this,
			SIGNAL(pan_changed(int)),
			model,
			SLOT(set_pan(int)));
	QObject::connect(
			model,
			SIGNAL(pan_changed(int)),
			this,
			SLOT(set_pan(int)));
}

void VCAView::set_level(int val){
	mLevel->setValue(val);
}

void VCAView::set_env_amount(int val){
	mEnvAmount->setValue(val);
}

void VCAView::set_attack(int val){
	mAttack->setValue(val);
}

void VCAView::set_decay(int val){
	mDecay->setValue(val);
}

void VCAView::set_sustain(int val){
	mSustain->setValue(val);
}

void VCAView::set_release(int val){
	mRelease->setValue(val);
}

void VCAView::set_velocity(int val){
	mVelocity->setValue(val);
}

void VCAView::set_pan(int val){
	mPanType->setCurrentIndex(val);
}


