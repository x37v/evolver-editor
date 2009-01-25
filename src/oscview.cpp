#include "oscview.hpp"
#include "oscmodel.hpp"
#include "sliderspinbox.hpp"
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QHBoxLayout>

OscView::OscView(QWidget * parent) : QWidget(parent){
	//allocate
	mFreqSlider = new SliderSpinBox(this);
	mTuneSlider = new SliderSpinBox(this);
	mGlideMode = new QComboBox(this);
	mGlideSlider = new SliderSpinBox(this);
	mLevelSlider = new SliderSpinBox(this);

	//set up
	mFreqSlider->setRange(0, OscModel::freq_max);
	mTuneSlider->setRange(OscModel::tune_min, OscModel::tune_max);
	mGlideSlider->setRange(0, OscModel::glide_max);
	mLevelSlider->setRange(0, OscModel::level_max);
	mGlideMode->addItem("normal");
	mGlideMode->addItem("fingered");
	mGlideMode->addItem("off");

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
	QObject::connect(mGlideMode,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(glide_mode_changed(int)));

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

	lab = new QLabel(this); lab->setText(QString("glide mode"));
	mLables.push_back(lab);
	mLayout->addWidget(lab, 2, 0, Qt::AlignRight);
	mLayout->addWidget(mGlideMode, 2, 1);

	lab = new QLabel(this); lab->setText(QString("glide"));
	mLables.push_back(lab);
	mLayout->addWidget(lab, 3, 0, Qt::AlignRight);
	mLayout->addWidget(mGlideSlider, 3, 1);

	lab = new QLabel(this); lab->setText(QString("level"));
	mLables.push_back(lab);
	mLayout->addWidget(lab, 4, 0, Qt::AlignRight);
	mLayout->addWidget(mLevelSlider, 4, 1);

	mLayout->setContentsMargins(2,2,1,1);
	mLayout->setSpacing(1);
	setLayout(mLayout);
}

void OscView::connect_to_model(OscModel * model){

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
			SIGNAL(tune_changed(int)),
			model,
			SLOT(set_tune(int)));
	QObject::connect(
			model,
			SIGNAL(tune_changed(int)),
			this,
			SLOT(set_tune(int)));

	QObject::connect(
			this,
			SIGNAL(glide_changed(int)),
			model,
			SLOT(set_glide(int)));
	QObject::connect(
			model,
			SIGNAL(glide_changed(int)),
			this,
			SLOT(set_glide(int)));

	QObject::connect(
			this,
			SIGNAL(glide_mode_changed(int)),
			model,
			SLOT(set_glide_mode(int)));
	QObject::connect(
			model,
			SIGNAL(glide_mode_changed(int)),
			this,
			SLOT(set_glide_mode(int)));

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

void OscView::set_glide_mode(int mode){
	mGlideMode->setCurrentIndex(mode);
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
	mLayout->addWidget(lab, 5, 0, Qt::AlignRight);
	hlayout->addWidget(mShapeSelect);
	hlayout->addWidget(mSyncButton);
	mLayout->addLayout(hlayout, 5, 1);

	lab = new QLabel(parent); lab->setText(QString("pulse width"));
	mLables.push_back(lab);
	mLayout->addWidget(lab, 6, 0, Qt::AlignRight);
	mLayout->addWidget(mWidthSlider, 6, 1);

	//connect out signals
	QObject::connect(mShapeSelect,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(shape_changed(int)));
	QObject::connect(mWidthSlider,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(width_changed(int)));
	QObject::connect(mSyncButton,
			SIGNAL(toggled(bool)),
			this,
			SIGNAL(sync_changed(bool)));
}

void AnalogOscView::connect_to_model(AnalogOscModel * model){
	//connect our parent
	OscView::connect_to_model(model);

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
			SIGNAL(width_changed(int)),
			model,
			SLOT(set_width(int)));
	QObject::connect(
			model,
			SIGNAL(width_changed(int)),
			this,
			SLOT(set_width(int)));

	QObject::connect(
			this,
			SIGNAL(sync_changed(bool)),
			model,
			SLOT(set_sync(bool)));
	QObject::connect(
			model,
			SIGNAL(sync_changed(bool)),
			this,
			SLOT(set_sync(bool)));
}

void AnalogOscView::show_sync_button(bool show){
	mSyncButton->setVisible(show);
}

void AnalogOscView::set_shape(int shape){
	mShapeSelect->setCurrentIndex(shape);
}

void AnalogOscView::set_width(int width){
	mWidthSlider->setValue(width);
}

void AnalogOscView::set_sync(bool s){
	mSyncButton->setChecked(s);
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
	mLayout->addWidget(lab, 5, 0, Qt::AlignRight);
	mLayout->addWidget(mShapeSlider, 5, 1);

	lab = new QLabel(parent); lab->setText(QString("fm in"));
	mLables.push_back(lab);
	mLayout->addWidget(lab, 6, 0, Qt::AlignRight);
	mLayout->addWidget(mFmInSlider, 6, 1);

	lab = new QLabel(parent); lab->setText(QString("ring in"));
	mLables.push_back(lab);
	mLayout->addWidget(lab, 7, 0, Qt::AlignRight);
	mLayout->addWidget(mRingInSlider, 7, 1);

	mShapeSeqSelect->addItem(QString("none"));
	mShapeSeqSelect->addItem(QString("seq 1"));
	mShapeSeqSelect->addItem(QString("seq 2"));
	mShapeSeqSelect->addItem(QString("seq 3"));
	mShapeSeqSelect->addItem(QString("seq 4"));

	lab = new QLabel(parent); lab->setText(QString("shape seq"));
	mLables.push_back(lab);
	mLayout->addWidget(lab, 8, 0, Qt::AlignRight);
	mLayout->addWidget(mShapeSeqSelect, 8, 1);

	//connect out signals
	QObject::connect(mShapeSlider,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(shape_changed(int)));
	QObject::connect(mFmInSlider,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(fm_in_changed(int)));
	QObject::connect(mRingInSlider,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(ring_in_changed(int)));
	QObject::connect(mShapeSeqSelect,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(shape_mod_changed(int)));
}

void DigitalOscView::connect_to_model(DigitalOscModel * model){
	//connect our parent
	OscView::connect_to_model(model);

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
			SIGNAL(fm_in_changed(int)),
			model,
			SLOT(set_fm_in(int)));
	QObject::connect(
			model,
			SIGNAL(fm_in_changed(int)),
			this,
			SLOT(set_fm_in(int)));

	QObject::connect(
			this,
			SIGNAL(ring_in_changed(int)),
			model,
			SLOT(set_ring_in(int)));
	QObject::connect(
			model,
			SIGNAL(ring_in_changed(int)),
			this,
			SLOT(set_ring_in(int)));

	QObject::connect(
			this,
			SIGNAL(shape_mod_changed(int)),
			model,
			SLOT(set_shape_mod(int)));
	QObject::connect(
			model,
			SIGNAL(shape_mod_changed(int)),
			this,
			SLOT(set_shape_mod(int)));
}

void DigitalOscView::set_shape(int shape){
	mShapeSlider->setValue(shape);
}

void DigitalOscView::set_fm_in(int fm){
	mFmInSlider->setValue(fm);
}

void DigitalOscView::set_ring_in(int ring){
	mRingInSlider->setValue(ring);
}

void DigitalOscView::set_shape_mod(int mod){
	mShapeSeqSelect->setCurrentIndex(mod);
}

