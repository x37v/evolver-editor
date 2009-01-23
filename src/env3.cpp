#include "env3.hpp"
#include "modulation_destinations.hpp"

const int Env3Model::amount_min = -99;
const int Env3Model::amount_max = 99;
const unsigned int Env3Model::delay_max = 110;
const unsigned int Env3Model::attack_max = 110;
const unsigned int Env3Model::decay_max = 110;
const unsigned int Env3Model::sustain_max = 100;
const unsigned int Env3Model::release_max = 110;
const unsigned int Env3Model::velocity_max = 100;

Env3Model::Env3Model(QObject * parent) : QObject(parent){
	mDestination = 0;
	mDelay = 0;
	mAmount = 0;
	mAttack = 0;
	mDecay = 0;
	mSustain = sustain_max;
	mRelease = release_max;
	mVelocity = 0;
}

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include "sliderspinbox.hpp"

Env3View::Env3View(QWidget * parent) : QWidget(parent){
	QLabel * lab;
	//allocate
	mLayout = new QGridLayout(this);

	mDestSelect = new ModDestComboBox(this);

	mDelay = new SliderSpinBox(this);
	mAmount = new SliderSpinBox(this);
	mAttack = new SliderSpinBox(this);
	mDecay = new SliderSpinBox(this);
	mSustain = new SliderSpinBox(this);
	mRelease = new SliderSpinBox(this);
	mVelocity = new SliderSpinBox(this);

	//set up
	mDelay->setRange(0, Env3Model::delay_max);
	mAmount->setRange(Env3Model::amount_min, Env3Model::amount_max);
	mAttack->setRange(0, Env3Model::attack_max);
	mDecay->setRange(0, Env3Model::decay_max);
	mSustain->setRange(0, Env3Model::sustain_max);
	mRelease->setRange(0, Env3Model::release_max);
	mVelocity->setRange(0, Env3Model::velocity_max);

	//label and plot
	lab = new QLabel(QString("delay"));
	mLayout->addWidget(mDelay, 0, 1);
	mLayout->addWidget(lab, 0, 0, Qt::AlignRight);

	lab = new QLabel(QString("amount"));
	mLayout->addWidget(mAmount, 1, 1);
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

	lab = new QLabel(QString("dest"));
	mLayout->addWidget(mDestSelect, 7, 1);
	mLayout->addWidget(lab, 7, 0, Qt::AlignRight);

	mLayout->setSpacing(2);
	mLayout->setRowStretch(10, 1);

	setLayout(mLayout);
}

