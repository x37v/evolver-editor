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

unsigned int LFOModel::freq(){
	return mFreq;
}

unsigned int LFOModel::amount(){
	return mAmount;
}

LFOModel::shape_t LFOModel::shape(){
	return mShape;
}

bool LFOModel::key_sync(){
	return mKeySync;
}

LFOModel::sync_types LFOModel::sync_type(){
	return mSyncType;
}

unsigned int LFOModel::destination(){
	return mDestination;
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

void LFOModel::set_shape(int s){
	shape_t shape = (shape_t)s;
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

void LFOModel::set_sync_type(int type){
	sync_types t = (sync_types)type;
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

	QObject::connect(mFreqSlider,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(freq_changed(int)));
	QObject::connect(mAmountSlider,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(amount_changed(int)));
	QObject::connect(mShapeSelect,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(shape_changed(int)));
	QObject::connect(mKeySyncButton,
			SIGNAL(clicked(bool)),
			this,
			SIGNAL(key_sync_changed(bool)));
	QObject::connect(mSyncSelect,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(sync_type_changed(int)));
	QObject::connect(mDestSelect,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(destination_changed(int)));
}

std::vector<QLabel *> * LFOView::labels(){
	return &mLabels;
}

void LFOView::show_labels(bool show){
	for(std::vector<QLabel *>::iterator it = mLabels.begin(); it != mLabels.end(); it++)
		(*it)->setVisible(show);
}

void LFOView::connect_to_model(LFOModel * model){
	QObject::connect(
			this,
			SIGNAL(freq_changed(int)),
			model,
			SLOT(set_freq(int)));
	QObject::connect(
			model,
			SIGNAL(freq_changed(int)),
			this,
			SLOT(set_freq(int)));

	QObject::connect(
			this,
			SIGNAL(amount_changed(int)),
			model,
			SLOT(set_amount(int)));
	QObject::connect(
			model,
			SIGNAL(amount_changed(int)),
			this,
			SLOT(set_amount(int)));

	QObject::connect(
			this,
			SIGNAL(shape_changed(int)),
			model,
			SLOT(set_shape(int)));
	QObject::connect(
			model,
			SIGNAL(shape_changed(int)),
			this,
			SLOT(set_shape(int)));

	QObject::connect(
			this,
			SIGNAL(key_sync_changed(bool)),
			model,
			SLOT(set_key_sync(bool)));
	QObject::connect(
			model,
			SIGNAL(key_sync_changed(bool)),
			this,
			SLOT(set_key_sync(bool)));

	QObject::connect(
			this,
			SIGNAL(sync_type_changed(int)),
			model,
			SLOT(set_sync_type(int)));
	QObject::connect(
			model,
			SIGNAL(sync_type_changed(int)),
			this,
			SLOT(set_sync_type(int)));

	QObject::connect(
			this,
			SIGNAL(destination_changed(int)),
			model,
			SLOT(set_destination(int)));
	QObject::connect(
			model,
			SIGNAL(destination_changed(int)),
			this,
			SLOT(set_destination(int)));
}

void LFOView::set_freq(int freq){
	mFreqSlider->setValue(freq);
}

void LFOView::set_amount(int amount){
	mAmountSlider->setValue(amount);
}

void LFOView::set_shape(int shape){
	mShapeSelect->setCurrentIndex(shape);
}

void LFOView::set_key_sync(bool sync){
	mKeySyncButton->setChecked(sync);
}

void LFOView::set_sync_type(int t){
	mSyncSelect->setCurrentIndex(t);
}

void LFOView::set_destination(int dest){
	mDestSelect->setCurrentIndex(dest);
}

