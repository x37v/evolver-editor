#include "feedback.hpp"
#include "common.hpp"

const unsigned int FeedbackModel::freq_max = 48;
const unsigned int FeedbackModel::level_max = 100;

FeedbackModel::FeedbackModel(QObject * parent) : Model(parent){
	mFreq = 0;
	mLevel = 0;
	mGrunge = false;
}

void FeedbackModel::set_freq(int f){
	if(in_range_and_new<unsigned int>((unsigned int)f, mFreq, freq_max)){
		mFreq = f;
		emit(freq_changed(mFreq));
		send_program_param(32, mFreq);
	}
}

void FeedbackModel::set_level(int l){
	if(in_range_and_new<unsigned int>((unsigned int)l, mLevel, level_max)){
		mLevel = l;
		emit(level_changed(mLevel));
		send_program_param(33, mLevel);
	}
}

void FeedbackModel::set_grunge(bool g){
	if(g != mGrunge){
		mGrunge = g;
		emit(grunge_changed(mGrunge));
		send_program_param(34, mGrunge);
	}
}

#include <QPushButton>
#include <QGridLayout>
#include <QLabel>
#include "sliderspinbox.hpp"

FeedbackView::FeedbackView(QWidget * parent) : QWidget(parent){
	QLabel * lab;
	//allocate
	mLayout = new QGridLayout(this);

	mFreq = new SliderSpinBox(this);
	mLevel = new SliderSpinBox(this);
	mGrunge = new QPushButton("grunge", this);

	//setup
	mFreq->setRange(0, FeedbackModel::freq_max);
	mLevel->setRange(0, FeedbackModel::level_max);
	mGrunge->setCheckable(true);

	//label and plot
	lab = new QLabel(QString("freq"));
	mLayout->addWidget(mFreq, 0, 1);
	mLayout->addWidget(lab, 0, 0, Qt::AlignRight);

	lab = new QLabel(QString("level"));
	mLayout->addWidget(mLevel, 1, 1);
	mLayout->addWidget(lab, 1, 0, Qt::AlignRight);

	mLayout->addWidget(mGrunge, 2, 1);

	mLayout->setSpacing(2);
	mLayout->setRowStretch(3, 1);

	setLayout(mLayout);

	//connect out signals
	QObject::connect(mFreq,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(freq_changed(int)));
	QObject::connect(mLevel,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(level_changed(int)));
	QObject::connect(mGrunge,
			SIGNAL(toggled(bool)),
			this,
			SIGNAL(grunge_changed(bool)));
}

void FeedbackView::connect_to_model(FeedbackModel * model){

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
			SIGNAL(grunge_changed(bool)),
			model,
			SLOT(set_grunge(bool)));
	QObject::connect(
			model,
			SIGNAL(grunge_changed(bool)),
			this,
			SLOT(set_grunge(bool)));
}

void FeedbackView::set_freq(int f){
	mFreq->setValue(f);
}

void FeedbackView::set_level(int l){
	mLevel->setValue(l);
}

void FeedbackView::set_grunge(bool g){
	mGrunge->setChecked(g);
}


