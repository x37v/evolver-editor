#include "lfo.hpp"
#include "common.hpp"
#include "modulation_destinations.hpp"
#include "modulators.hpp"

const unsigned int LFOModel::freq_max = 150;
const unsigned int LFOModel::amount_max = 100;

LFOModel::LFOModel(QObject * parent) : QObject(parent){
	mShape = triangle;
	mFreq = 0;
	mAmount = 0;
	mKeySync = false;
	mSyncType = off;
	mDestination = 0;
}

void LFOModel::set_freq(int freq){
	if(in_range_and_new<unsigned int>((unsigned int)freq, mFreq, freq_max)){
		mFreq = (unsigned int)freq;
		emit(freq_changed(mFreq));
	}
}

void LFOModel::set_amount(int amount){
	if(in_range_and_new<unsigned int>((unsigned int)amount, mAmount, amount_max)){
		mAmount = (unsigned int)amount;
		emit(amount_changed(mAmount));
	}
}

void LFOModel::set_shape(shapes shape){
	if(shape != mShape){
		mShape = shape;
		emit(shape_changed(mShape));
	}
}

void LFOModel::set_key_sync(bool sync){
	if(sync != mKeySync){
		mKeySync = sync;
		emit(key_sync_changed(mKeySync));
	}
}

void LFOModel::set_sync_type(sync_types t){
	if(t != mSyncType){
		mSyncType = t;
		emit(sync_type_changed(t));
	}
}

void LFOModel::set_destination(int dest){
	if(dest >= 0 && dest < NUM_MODULATION_DESTINATIONS && (unsigned int)dest != mDestination){
		mDestination = dest;
		emit(destination_changed(mDestination));
	}
}


#include <QComboBox>
#include <QString>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include "sliderspinbox.hpp"

LFOView::LFOView(QWidget * parent) : QWidget(parent){
	mLayout = new QGridLayout(this);

	mKeySyncButton = new QPushButton(this);
	mKeySyncButton->setCheckable(true);

	mAmountSlider = new SliderSpinBox(this);
	mFreqSlider = new SliderSpinBox(this);
	mAmountSlider->setRange(0, LFOModel::amount_max);
	mFreqSlider->setRange(0, LFOModel::freq_max);

	QLabel * lab;
	lab = new QLabel(this); lab->setText(QString("amount"));
	mLabels.push_back(lab);
	mLayout->addWidget(lab, 0, 0, Qt::AlignRight);
	mLayout->addWidget(mAmountSlider, 0, 1);

	lab = new QLabel(this); lab->setText(QString("freq"));
	mLabels.push_back(lab);
	mLayout->addWidget(lab, 1, 0, Qt::AlignRight);
	mLayout->addWidget(mFreqSlider, 1 , 1);


	mDestSelect = new ModDestComboBox(this);
	mSyncSelect = new QComboBox(this);

	mSyncSelect->addItem(QString("free"));
	mSyncSelect->addItem(QString("seq div 32"));
	mSyncSelect->addItem(QString("seq div 16"));
	mSyncSelect->addItem(QString("seq div 8"));
	mSyncSelect->addItem(QString("seq div 4"));
	mSyncSelect->addItem(QString("seq div 2"));
	mSyncSelect->addItem(QString("1 per step"));
	mSyncSelect->addItem(QString("2 per step"));
	mSyncSelect->addItem(QString("4 per step"));
	mSyncSelect->addItem(QString("8 per step"));
	mSyncSelect->addItem(QString("16 per step"));

	mShapeSelect = new QComboBox(this);
	mShapeSelect->addItem("tri");
	mShapeSelect->addItem("rev saw");
	mShapeSelect->addItem("saw");
	mShapeSelect->addItem("square");
	mShapeSelect->addItem("random");

	lab = new QLabel(this); lab->setText(QString("sync select"));
	mLabels.push_back(lab);
	mLayout->addWidget(lab, 2, 0, Qt::AlignRight);
	mLayout->addWidget(mSyncSelect, 2, 1);

	lab = new QLabel(this); lab->setText(QString("shape select"));
	mLabels.push_back(lab);
	mLayout->addWidget(lab, 3, 0, Qt::AlignRight);
	mLayout->addWidget(mShapeSelect, 3, 1);


	lab = new QLabel(this); lab->setText(QString("destination"));
	mLabels.push_back(lab);
	mLayout->addWidget(lab, 4, 0, Qt::AlignRight);
	mLayout->addWidget(mDestSelect, 4, 1);

	mKeySyncButton->setText(QString("key sync"));
	mLayout->addWidget(mKeySyncButton, 5, 1);

	mLayout->setSpacing(2);
	mLayout->setContentsMargins(1,1,1,1);

	setLayout(mLayout);

	//connect out signals

	QObject::connect(mDestSelect,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(modulation_dest_changed(int)));
	QObject::connect(mSyncSelect,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(sync_changed(int)));
	QObject::connect(mShapeSelect,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(shape_changed(int)));
	QObject::connect(mFreqSlider,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(freq_changed(int)));
	QObject::connect(mAmountSlider,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(amount_changed(int)));
	QObject::connect(mKeySyncButton,
			SIGNAL(clicked(bool)),
			this,
			SIGNAL(key_sync_changed(bool)));
}

std::vector<QLabel *> * LFOView::labels(){
	return &mLabels;
}

void LFOView::show_labels(bool show){
	for(std::vector<QLabel *>::iterator it = mLabels.begin(); it != mLabels.end(); it++)
		(*it)->setVisible(show);
}


LFOArrayView::LFOArrayView(QWidget * parent) : QWidget(parent){
	mLayout = new QVBoxLayout(this);
	for(unsigned int i = 0; i < 4; i++){
		LFOView * lfo = new LFOView(this);
		mLFOs.push_back(lfo);
		mLayout->addWidget(lfo);
	}
	setLayout(mLayout);
}

LFOView * LFOArrayView::operator[] (const int index){
	if(index < 0 || (unsigned int)index >= mLFOs.size())
		throw(std::runtime_error("array index out of bounds"));
	return mLFOs[index];
}
