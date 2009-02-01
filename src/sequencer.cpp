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

#include "sequencer.hpp"
#include "sliderspinbox.hpp"
#include <QSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QToolButton>
#include <QSignalMapper>
#include "modulation_destinations.hpp"

SequencerModel::SequencerModel(QObject * parent) : Model(parent){
	//init the data
	for(unsigned int i = 0; i < 4; i++){
		mLength[i] = 16;
		mDestination[i] = 0;
		for(unsigned int j = 0; j < 16; j++){
			mSequence[i][j] = 0;
			mSequenceReset[i][j] = false;
			mRest[j] = false;
		}
	}
}

unsigned int SequencerModel::length(unsigned int seq){
	if(seq > 3)
		return 0;
	return mLength[seq];
}

unsigned int SequencerModel::first_sequence_reset(unsigned int seq, unsigned int start){
	if(seq > 3)
		return 0;
	for(unsigned int i = start; i < 16; i++){
		if(mSequenceReset[seq][i])
			return i;
	}
	return 16;
}

void SequencerModel::insert_sequence_reset(unsigned int seq, unsigned int step){
	if(seq > 3 || step > 15)
		return;
	mSequenceReset[seq][step] = true;
}

void SequencerModel::remove_sequence_reset(unsigned int seq, unsigned int step){
	if(seq > 3 || step > 15)
		return;
	mSequenceReset[seq][step] = false;
}

void SequencerModel::clear_sequence_resets(){
	for(unsigned int i = 0; i < 4; i++){
		for(unsigned int j = 0; j < 16; j++)
			mSequenceReset[i][j] = false;
	}
}

void SequencerModel::set_length(unsigned int seq, unsigned int length){
	if(seq > 3 || length > 16)
		return;
	if(mLength[seq] != length){
		mLength[seq] = length;
		emit(length_changed(seq, mLength[seq]));
		//send all the values up until length
		//seq 0 has the rests
		if(seq == 0){
			//if our new length was on a rest, this is no longer a rest
			if(length < 16)
				set_rest(length, false);
			for(unsigned int i = 0; i < length; i++){
				if(i < 16){
					if(mRest[i])
						send_sequencer_param(i + seq * 16, 102);
					else
						send_sequencer_param(i + seq * 16, mSequence[seq][i]);
					mSequenceReset[seq][i] = false;
				}
			}
		} else {
			for(unsigned int i = 0; i < length; i++){
				if(i < 16) {
					send_sequencer_param(i + seq * 16, mSequence[seq][i]);
					mSequenceReset[seq][i] = false;
				}
			}
		}
		if(length != 16)
			send_sequencer_param(length + seq * 16, 101);
	}
	//this is a sequence reset point
	if(length < 15)
		mSequenceReset[seq][length] = true;
}

void SequencerModel::set_rest(unsigned int step, bool rest){
	if(step > 15)
		return;
	if(mRest[step] != rest){
		//this is a no longer a sequence reset point
		mSequenceReset[0][step] = false;

		//if this rest was the reset point for the first sequence, 
		//our length is no longer valid
		//reset our length
		if(rest && step == mLength[0])
			set_length(0, first_sequence_reset(0));

		mRest[step] = rest;
		emit(rest_changed(step, mRest[step]));
		if(rest)
			send_sequencer_param(step, 102);
		else
			send_sequencer_param(step, mSequence[0][step]);
	}
}

void SequencerModel::set_value(unsigned int seq, unsigned int step, unsigned int value){
	if(seq > 3 || step > 15 || value > 100)
		return;
	if(mSequence[seq][step] != value){
		mSequence[seq][step] = value;
		emit(value_changed(seq, step, mSequence[seq][step]));

		//this is a no longer a sequence reset point
		mSequenceReset[seq][step] = false;

		//deal with length and rests
		if((mRest[step] && seq == 0) || mLength[seq] <= step)
			return;
		send_sequencer_param(step + seq * 16, value);
	}
}

void SequencerModel::set_destination(unsigned int seq, unsigned int index){
	if(seq > 3 || index >= NUM_SEQ_MODULATION_DESTINATIONS)
		return;
	if(index != mDestination[seq]){
		mDestination[seq] = index;
		emit(destination_changed(seq, mDestination[seq]));
		send_program_param(56 + seq, index);
	}
}

SequencerView::SequencerView(QWidget * parent) : QWidget(parent){
	QSignalMapper * lengthMap = new QSignalMapper(this);
	QSignalMapper * restMap = new QSignalMapper(this);
	QSignalMapper * valueMap = new QSignalMapper(this);
	QSignalMapper * destMap = new QSignalMapper(this);

	mLayout = new QGridLayout(this);
	QGridLayout * destLayout = new QGridLayout;
	QLabel * lab;
	lab = new QLabel("sequence modulation destinations", this);
	destLayout->addWidget(lab, 0, 0, 1, 4, Qt::AlignCenter);

	for(unsigned int i = 0; i < 4; i++){
		mDestination[i] = new ModDestComboBox(this, true);
		lab = new QLabel(QString("sequencer %1").arg(i + 1), this);
		destLayout->addWidget(lab, 1, i, Qt::AlignCenter);
		destLayout->addWidget(mDestination[i], 2, i, Qt::AlignCenter);

		mLength[i] = new QSpinBox(this);
		mLength[i]->setRange(0,16);
		mLength[i]->setValue(16);
		mLayout->addWidget(mLength[i], i + 1, 0, Qt::AlignTop | Qt::AlignHCenter);
		for(unsigned int j = 0; j < 16; j++){
			mSequence[i][j] = new SliderSpinBox(this, true);
			mSequence[i][j]->setRange(0,100);
			mLayout->addWidget(mSequence[i][j], i + 1, j + 1, Qt::AlignCenter);
			QObject::connect(mSequence[i][j], SIGNAL(valueChanged(int)), valueMap, SLOT(map()));
			valueMap->setMapping(mSequence[i][j], i * 16 + j);
		}
		QObject::connect(mDestination[i], SIGNAL(currentIndexChanged(int)), destMap, SLOT(map()));
		QObject::connect(mLength[i], SIGNAL(valueChanged(int)), lengthMap, SLOT(map()));
		destMap->setMapping(mDestination[i], i);
		lengthMap->setMapping(mLength[i], i);
	}
	QObject::connect(valueMap, SIGNAL(mapped(int)), this, SLOT(local_set_value(int)));
	QObject::connect(destMap, SIGNAL(mapped(int)), this, SLOT(local_set_destination(int)));
	QObject::connect(lengthMap, SIGNAL(mapped(int)), this, SLOT(local_set_length(int)));

	for(unsigned int i = 0; i < 16; i++){
		mRest[i] = new QToolButton(this);
		mRest[i]->setCheckable(true);
		mRest[i]->setText("rest");
		mLayout->addWidget(mRest[i], 5, i + 1, Qt::AlignCenter);
		QObject::connect(mRest[i], SIGNAL(clicked(bool)), restMap, SLOT(map()));
		restMap->setMapping(mRest[i], i);
	}
	QObject::connect(restMap, SIGNAL(mapped(int)), this, SLOT(local_set_rest(int)));
	mLayout->addLayout(destLayout, 6, 1, 2, 16, Qt::AlignCenter);

	lab = new QLabel("length", this);
	mLayout->addWidget(lab, 0, 0, Qt::AlignHCenter);
	mLayout->setRowStretch(6, 10);
	mLayout->setColumnStretch(20, 10);
	setLayout(mLayout);
}

void SequencerView::connect_to_model(SequencerModel * model){
	QObject::connect(
			this,
			SIGNAL(length_changed(unsigned int, unsigned int)),
			model,
			SLOT(set_length(unsigned int, unsigned int)));
	QObject::connect(
			model,
			SIGNAL(length_changed(unsigned int, unsigned int)),
			this,
			SLOT(set_length(unsigned int, unsigned int)));

	QObject::connect(
			this,
			SIGNAL(rest_changed(unsigned int, bool)),
			model,
			SLOT(set_rest(unsigned int, bool)));
	QObject::connect(
			model,
			SIGNAL(rest_changed(unsigned int, bool)),
			this,
			SLOT(set_rest(unsigned int, bool)));

	QObject::connect(
			this,
			SIGNAL(value_changed(unsigned int, unsigned int, unsigned int)),
			model,
			SLOT(set_value(unsigned int, unsigned int, unsigned int)));
	QObject::connect(
			model,
			SIGNAL(value_changed(unsigned int, unsigned int, unsigned int)),
			this,
			SLOT(set_value(unsigned int, unsigned int, unsigned int)));

	QObject::connect(
			this,
			SIGNAL(destination_changed(unsigned int, unsigned int)),
			model,
			SLOT(set_destination(unsigned int, unsigned int)));
	QObject::connect(
			model,
			SIGNAL(destination_changed(unsigned int, unsigned int)),
			this,
			SLOT(set_destination(unsigned int, unsigned int)));
}

void SequencerView::set_length(unsigned int seq, unsigned int length){
	if(seq > 3 || length > 16)
		return;
	for(unsigned int i = 0; i < length; i++)
		mSequence[seq][i]->setEnabled(true);
	for(unsigned int i = length; i < 16; i++)
		mSequence[seq][i]->setEnabled(false);
	mLength[seq]->setValue(length);
}

void SequencerView::set_rest(unsigned int step, bool rest){
	if(step > 15)
		return;
	mRest[step]->setChecked(rest);
}

void SequencerView::set_value(unsigned int seq, unsigned int step, unsigned int value){
	if(seq > 3 || step > 15)
		return;
	mSequence[seq][step]->setValue(value);
}

void SequencerView::set_destination(unsigned int seq, unsigned int index){
	if(seq > 3 || index >= (unsigned int)mDestination[seq]->count())
		return;
	mDestination[seq]->setCurrentIndex(index);
}

//these are internal slots which set up mappings of signal outputs
void SequencerView::local_set_length(int index){
	if(index < 4)
		emit(length_changed(index, mLength[index]->value()));
}

void SequencerView::local_set_rest(int index){
	if(index < 16)
		emit(rest_changed(index, mRest[index]->isChecked()));
}

void SequencerView::local_set_value(int index){
	unsigned int seq = index / 16;
	unsigned int step = index % 16;
	if(seq < 4)
		emit(value_changed(seq, step, mSequence[seq][step]->value()));
}

void SequencerView::local_set_destination(int index){
	if(index < 4)
		emit(destination_changed(index, mDestination[index]->currentIndex()));
}

