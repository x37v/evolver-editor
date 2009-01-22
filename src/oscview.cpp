#include "oscview.hpp"
#include "oscmodel.hpp"
#include "sliderspinbox.hpp"
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>

OscView::OscView(QWidget * parent) : QWidget(parent){
	mFreqSlider = new SliderSpinBox(this);
	mTuneSlider = new SliderSpinBox(this);
	mGlideSlider = new SliderSpinBox(this);
	mLevelSlider = new SliderSpinBox(this);

	mFreqSlider->setRange(0, OscModel::freq_max);
	mTuneSlider->setRange(OscModel::tune_min, OscModel::tune_max);
	mGlideSlider->setRange(0, OscModel::glide_max);
	mLevelSlider->setRange(0, OscModel::level_max);

	//connect signals out
	QObject::connect(mFreqSlider,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(freq_changed(int)));
	QObject::connect(mTuneSlider,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(tune_changed(int)));
	QObject::connect(mGlideSlider,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(glide_changed(int)));
	QObject::connect(mLevelSlider,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(level_changed(int)));

	mLayout = new QGridLayout(this);

	QLabel * lab;
	lab = new QLabel(this); lab->setText(QString("freq"));
	mLables.push_back(lab);
	mLayout->addWidget(lab, 0, 0, Qt::AlignRight);
	mLayout->addWidget(mFreqSlider, 0, 1);

	lab = new QLabel(this); lab->setText(QString("tune"));
	mLables.push_back(lab);
	mLayout->addWidget(lab, 1, 0, Qt::AlignRight);
	mLayout->addWidget(mTuneSlider, 1, 1);

	lab = new QLabel(this); lab->setText(QString("glide"));
	mLables.push_back(lab);
	mLayout->addWidget(lab, 2, 0, Qt::AlignRight);
	mLayout->addWidget(mGlideSlider, 2, 1);

	lab = new QLabel(this); lab->setText(QString("level"));
	mLables.push_back(lab);
	mLayout->addWidget(lab, 3, 0, Qt::AlignRight);
	mLayout->addWidget(mLevelSlider, 3, 1);

	mLayout->setContentsMargins(2,2,1,1);
	mLayout->setSpacing(1);
	setLayout(mLayout);
}

std::vector<QLabel *> * OscView::labels(){
	return &mLables;
}

void OscView::set_freq(int freq){
	mFreqSlider->setValue(freq);
}

void OscView::set_tune(int tune){
	mTuneSlider->setValue(tune);
}

void OscView::set_glide(int glide){
	mGlideSlider->setValue(glide);
}

void OscView::set_level(int lev){
	mLevelSlider->setValue(lev);
}

void OscView::show_labels(bool show){
	for(std::vector<QLabel *>::iterator it = mLables.begin(); it != mLables.end(); it++)
		(*it)->setVisible(show);
}

AnalogOscView::AnalogOscView(QWidget * parent) : OscView(parent) {

	QLabel * lab;
	QHBoxLayout * hlayout = new QHBoxLayout;
	hlayout->setSpacing(2);
	hlayout->setContentsMargins(0,0,0,0);

	mWidthSlider = new SliderSpinBox(parent);
	mShapeSelect = new QComboBox(parent);
	mSyncButton = new QPushButton("sync 2 -> 1", this);
	mSyncButton->setCheckable(true);

	mWidthSlider->setRange(0, AnalogOscModel::width_max);
	mShapeSelect->addItem(QString("saw"));
	mShapeSelect->addItem(QString("triangle"));
	mShapeSelect->addItem(QString("saw/tri mix"));
	mShapeSelect->addItem(QString("pulse"));

	lab = new QLabel(parent); lab->setText(QString("shape"));
	mLables.push_back(lab);
	mLayout->addWidget(lab, 4, 0, Qt::AlignRight);
	hlayout->addWidget(mShapeSelect);
	hlayout->addWidget(mSyncButton);
	mLayout->addLayout(hlayout, 4, 1);

	lab = new QLabel(parent); lab->setText(QString("pulse width"));
	mLables.push_back(lab);
	mLayout->addWidget(lab, 5, 0, Qt::AlignRight);
	mLayout->addWidget(mWidthSlider, 5, 1);
}

void AnalogOscView::show_sync_button(bool show){
	mSyncButton->setVisible(show);
}

DigitalOscView::DigitalOscView(QWidget * parent) : OscView(parent){
	QLabel * lab;

	mShapeSlider = new SliderSpinBox(parent);
	mFmInSlider = new SliderSpinBox(parent);
	mRingInSlider = new SliderSpinBox(parent);
	mShapeSeqSelect = new QComboBox(parent);

	mShapeSlider->setRange(0, DigitalOscModel::shape_max);
	mFmInSlider->setRange(0, DigitalOscModel::fm_in_max);
	mRingInSlider->setRange(0, DigitalOscModel::ring_in_max);

	lab = new QLabel(parent); lab->setText(QString("shape"));
	mLables.push_back(lab);
	mLayout->addWidget(lab, 4, 0, Qt::AlignRight);
	mLayout->addWidget(mShapeSlider, 4, 1);

	lab = new QLabel(parent); lab->setText(QString("fm in"));
	mLables.push_back(lab);
	mLayout->addWidget(lab, 5, 0, Qt::AlignRight);
	mLayout->addWidget(mFmInSlider, 5, 1);

	lab = new QLabel(parent); lab->setText(QString("ring in"));
	mLables.push_back(lab);
	mLayout->addWidget(lab, 6, 0, Qt::AlignRight);
	mLayout->addWidget(mRingInSlider, 6, 1);

	mShapeSeqSelect->addItem(QString("none"));
	mShapeSeqSelect->addItem(QString("seq 1"));
	mShapeSeqSelect->addItem(QString("seq 2"));
	mShapeSeqSelect->addItem(QString("seq 3"));
	mShapeSeqSelect->addItem(QString("seq 4"));

	lab = new QLabel(parent); lab->setText(QString("shape seq"));
	mLables.push_back(lab);
	mLayout->addWidget(lab, 7, 0, Qt::AlignRight);
	mLayout->addWidget(mShapeSeqSelect, 7, 1);
}
