#include "feedback.hpp"

const unsigned int FeedbackModel::freq_max = 48;
const unsigned int FeedbackModel::level_max = 100;

FeedbackModel::FeedbackModel(QObject * parent) : QObject(parent){
	mFreq = 0;
	mLevel = 0;
	mGrunge = false;
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
}

