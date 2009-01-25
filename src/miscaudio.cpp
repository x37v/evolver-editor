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

#include "miscaudio.hpp"
#include "common.hpp"

const unsigned int MiscAudioModel::volume_max = 100;
const unsigned int MiscAudioModel::distortion_max = 99;
const unsigned int MiscAudioModel::hack_max = 14;
const unsigned int MiscAudioModel::noise_volume_max = 100;
const unsigned int MiscAudioModel::ext_in_volume_max = 100;

MiscAudioModel::MiscAudioModel(QObject * parent) : Model(parent){
	mVolume = 100;
	mDistortionType = internal;
	mDistortionAmount = 0;
	mOutputHack = 0;
	mInputHack = 0;
	mNoiseVolume = 0;
	mExtInMode = stereo;
	mExtInVolume = 0;
	mEnvCurve = exponential;
}

void MiscAudioModel::set_volume(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mVolume, volume_max)){
		mVolume = val;
		emit(volume_changed(mVolume));
		send_program_param(31, mVolume);
	}
}

void MiscAudioModel::set_distortion_type(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mDistortionType, 1)){
		mDistortionType = (distortion_type)val;
		emit(distortion_type_changed(mDistortionType));
		if(mDistortionType == internal)
			send_program_param(103, mDistortionAmount);
		else
			send_program_param(103, mDistortionAmount + 100);
	}
}

void MiscAudioModel::set_distortion_amount(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mDistortionAmount, distortion_max)){
		mDistortionAmount = val;
		emit(emit(distortion_amount_changed(mDistortionAmount)));
		if(mDistortionType == internal)
			send_program_param(103, mDistortionAmount);
		else
			send_program_param(103, mDistortionAmount + 100);
	}
}

void MiscAudioModel::set_output_hack(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mOutputHack, hack_max)){
		mOutputHack = val;
		emit(output_hack_changed(mOutputHack));
		send_program_param(39, mOutputHack);
	}
}

void MiscAudioModel::set_input_hack(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mInputHack, hack_max)){
		mInputHack = val;
		emit(input_hack_changed(mInputHack));
		send_program_param(63, mInputHack);
	}
}

void MiscAudioModel::set_noise_volume(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mNoiseVolume, noise_volume_max)){
		mNoiseVolume = val;
		emit(noise_volume_changed(mNoiseVolume));
		send_program_param(60, mNoiseVolume);
	}
}

void MiscAudioModel::set_ext_in_mode(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mExtInMode, 3)){
		mExtInMode = (ext_in_mode_type)val;
		emit(ext_in_mode_changed(mExtInMode));
		send_program_param(62, mExtInMode);
	}
}

void MiscAudioModel::set_ext_in_volume(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mExtInVolume, ext_in_volume_max)){
		mExtInVolume = val;
		emit(ext_in_volume_changed(mExtInVolume));
		send_program_param(61, mExtInVolume);
	}
}

void MiscAudioModel::set_env_curve(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mEnvCurve, 1)){
		mEnvCurve = (env_curve_type)val;
		emit(env_curve_changed(mEnvCurve));
		send_program_param(88, mEnvCurve);
	}
}


#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include "sliderspinbox.hpp"

MiscAudioView::MiscAudioView(QWidget * parent) : QWidget(parent){
	QLabel * lab;
	//allocate
	mLayout = new QGridLayout(this);
	mVolume = new SliderSpinBox(this);
	mDistortionType = new QComboBox(this);
	mDistortionAmount = new SliderSpinBox(this);
	mOutputHack = new SliderSpinBox(this);
	mInputHack = new SliderSpinBox(this);
	mNoiseVolume = new SliderSpinBox(this);
	mExtInMode = new QComboBox(this);
	mExtInVolume = new SliderSpinBox(this);
	mEnvCurve = new QComboBox(this);

	//setup
	mVolume->setRange(0, MiscAudioModel::volume_max);

	mDistortionType->addItem("internal");
	mDistortionType->addItem("input");
	mDistortionAmount->setRange(0, MiscAudioModel::distortion_max);
	mOutputHack->setRange(0, MiscAudioModel::hack_max);
	mInputHack->setRange(0, MiscAudioModel::hack_max);
	mNoiseVolume->setRange(0, MiscAudioModel::noise_volume_max);

	mExtInMode->addItem("stereo");
	mExtInMode->addItem("left");
	mExtInMode->addItem("right");
	mExtInMode->addItem("split");
	mExtInVolume->setRange(0, MiscAudioModel::ext_in_volume_max);

	mEnvCurve->addItem("exponential");
	mEnvCurve->addItem("linear");

	//label and plot
	lab = new QLabel(QString("volume"));
	mLayout->addWidget(mVolume, 0, 1);
	mLayout->addWidget(lab, 0, 0, Qt::AlignRight);

	lab = new QLabel(QString("dist type"));
	mLayout->addWidget(mDistortionType, 1, 1);
	mLayout->addWidget(lab, 1, 0, Qt::AlignRight);

	lab = new QLabel(QString("dist amt"));
	mLayout->addWidget(mDistortionAmount, 2, 1);
	mLayout->addWidget(lab, 2, 0, Qt::AlignRight);

	lab = new QLabel(QString("output hack"));
	mLayout->addWidget(mOutputHack, 3, 1);
	mLayout->addWidget(lab, 3, 0, Qt::AlignRight);

	lab = new QLabel(QString("input hack"));
	mLayout->addWidget(mInputHack, 4, 1);
	mLayout->addWidget(lab, 4, 0, Qt::AlignRight);

	lab = new QLabel(QString("noise vol"));
	mLayout->addWidget(mNoiseVolume, 5, 1);
	mLayout->addWidget(lab, 5, 0, Qt::AlignRight);

	lab = new QLabel(QString("ext in mode"));
	mLayout->addWidget(mExtInMode, 6, 1);
	mLayout->addWidget(lab, 6, 0, Qt::AlignRight);

	lab = new QLabel(QString("ext in vol"));
	mLayout->addWidget(mExtInVolume, 7, 1);
	mLayout->addWidget(lab, 7, 0, Qt::AlignRight);

	lab = new QLabel(QString("env curve"));
	mLayout->addWidget(mEnvCurve, 8, 1);
	mLayout->addWidget(lab, 8, 0, Qt::AlignRight);

	mLayout->setSpacing(2);
	mLayout->setRowStretch(10, 1);

	setLayout(mLayout);
	
	//connect out signals
	QObject::connect(mVolume,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(volume_changed(int)));
	QObject::connect(mDistortionType,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(distortion_type_changed(int)));
	QObject::connect(mDistortionAmount,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(distortion_amount_changed(int)));
	QObject::connect(mOutputHack,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(output_hack_changed(int)));
	QObject::connect(mInputHack,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(input_hack_changed(int)));
	QObject::connect(mNoiseVolume,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(noise_volume_changed(int)));
	QObject::connect(mExtInMode,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(ext_in_mode_changed(int)));
	QObject::connect(mExtInVolume,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(ext_in_volume_changed(int)));
	QObject::connect(mEnvCurve,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(env_curve_changed(int)));
}

void MiscAudioView::connect_to_model(MiscAudioModel * model){

	QObject::connect(
			this,
			SIGNAL(volume_changed(int)),
			model,
			SLOT(set_volume(int)));
	QObject::connect(
			model,
			SIGNAL(volume_changed(int)),
			this,
			SLOT(set_volume(int)));

	QObject::connect(
			this,
			SIGNAL(distortion_type_changed(int)),
			model,
			SLOT(set_distortion_type(int)));
	QObject::connect(
			model,
			SIGNAL(distortion_type_changed(int)),
			this,
			SLOT(set_distortion_type(int)));

	QObject::connect(
			this,
			SIGNAL(distortion_amount_changed(int)),
			model,
			SLOT(set_distortion_amount(int)));
	QObject::connect(
			model,
			SIGNAL(distortion_amount_changed(int)),
			this,
			SLOT(set_distortion_amount(int)));

	QObject::connect(
			this,
			SIGNAL(output_hack_changed(int)),
			model,
			SLOT(set_output_hack(int)));
	QObject::connect(
			model,
			SIGNAL(output_hack_changed(int)),
			this,
			SLOT(set_output_hack(int)));

	QObject::connect(
			this,
			SIGNAL(input_hack_changed(int)),
			model,
			SLOT(set_input_hack(int)));
	QObject::connect(
			model,
			SIGNAL(input_hack_changed(int)),
			this,
			SLOT(set_input_hack(int)));

	QObject::connect(
			this,
			SIGNAL(noise_volume_changed(int)),
			model,
			SLOT(set_noise_volume(int)));
	QObject::connect(
			model,
			SIGNAL(noise_volume_changed(int)),
			this,
			SLOT(set_noise_volume(int)));

	QObject::connect(
			this,
			SIGNAL(ext_in_mode_changed(int)),
			model,
			SLOT(set_ext_in_mode(int)));
	QObject::connect(
			model,
			SIGNAL(ext_in_mode_changed(int)),
			this,
			SLOT(set_ext_in_mode(int)));

	QObject::connect(
			this,
			SIGNAL(ext_in_volume_changed(int)),
			model,
			SLOT(set_ext_in_volume(int)));
	QObject::connect(
			model,
			SIGNAL(ext_in_volume_changed(int)),
			this,
			SLOT(set_ext_in_volume(int)));

	QObject::connect(
			this,
			SIGNAL(env_curve_changed(int)),
			model,
			SLOT(set_env_curve(int)));
	QObject::connect(
			model,
			SIGNAL(env_curve_changed(int)),
			this,
			SLOT(set_env_curve(int)));
}

void MiscAudioView::set_volume(int val){
	mVolume->setValue(val);
}

void MiscAudioView::set_distortion_type(int val){
	mDistortionType->setCurrentIndex(val);
}

void MiscAudioView::set_distortion_amount(int val){
	mDistortionAmount->setValue(val);
}

void MiscAudioView::set_output_hack(int val){
	mOutputHack->setValue(val);
}

void MiscAudioView::set_input_hack(int val){
	mInputHack->setValue(val);
}

void MiscAudioView::set_noise_volume(int val){
	mNoiseVolume->setValue(val);
}

void MiscAudioView::set_ext_in_mode(int val){
	mExtInMode->setCurrentIndex(val);
}

void MiscAudioView::set_ext_in_volume(int val){
	mExtInVolume->setValue(val);
}

void MiscAudioView::set_env_curve(int val){
	mEnvCurve->setCurrentIndex(val);
}

