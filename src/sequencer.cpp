#include "sequencer.hpp"
#include "sliderspinbox.hpp"
#include <QSpinBox>
#include <QGridLayout>
#include <QLabel>
#include <QToolButton>

SequencerModel::SequencerModel(QObject * parent) : Model(parent){
	//init the data
	for(unsigned int i = 0; i < 4; i++){
		mLength[i] = 16;
		for(unsigned int j = 0; j < 16; j++){
			mSequence[i][j] = 0;
			mRest[j] = false;
		}
	}
}

void SequencerModel::set_length(unsigned int seq, unsigned int length){
	if(seq > 3 || length > 16)
		return;
	if(mLength[seq] != length){
		mLength[seq] = length;
		emit(length_changed(seq, mLength[seq]));
	}
}

void SequencerModel::set_rest(unsigned int step, bool rest){
	if(step > 15)
		return;
	if(mRest[step] != rest){
		mRest[step] = rest;
		emit(rest_changed(step, mRest[step]));
	}
}

void SequencerModel::set_value(unsigned int seq, unsigned int step, unsigned int value){
	if(seq > 3 || step > 15 || value > 100)
		return;
	if(mSequence[seq][step] != value){
		mSequence[seq][step] = value;
		emit(value_changed(seq, step, mSequence[seq][step]));
	}
}

SequencerView::SequencerView(QWidget * parent) : QWidget(parent){
	mLayout = new QGridLayout(this);
	for(unsigned int i = 0; i < 4; i++){
		mLength[i] = new QSpinBox(this);
		mLength[i]->setRange(0,16);
		mLength[i]->setValue(16);
		mLayout->addWidget(mLength[i], i + 1, 0, Qt::AlignHCenter);
		for(unsigned int j = 0; j < 16; j++){
			mSequence[i][j] = new SliderSpinBox(this, true);
			mSequence[i][j]->setRange(0,100);
			mLayout->addWidget(mSequence[i][j], i + 1, j + 1, Qt::AlignCenter);
		}
	}

	for(unsigned int i = 0; i < 16; i++){
		mRest[i] = new QToolButton(this);
		mRest[i]->setCheckable(true);
		mRest[i]->setText("rest");
		mLayout->addWidget(mRest[i], 5, i + 1, Qt::AlignCenter);
	}

	QLabel * lab = new QLabel("length", this);
	mLayout->addWidget(lab, 0, 0, Qt::AlignHCenter);
	mLayout->setRowStretch(10, 10);
	setLayout(mLayout);
}

void SequencerView::set_length(unsigned int seq, unsigned int length){
	if(seq > 3 || length > 16)
		return;
	mLength[seq]->setValue(length);
}

void SequencerView::set_rest(unsigned int step, bool rest){
}

void SequencerView::set_value(unsigned int seq, unsigned int step, unsigned int value){
	if(seq > 3 || step > 15)
		return;
	mSequence[seq][step]->setValue(value);
}

