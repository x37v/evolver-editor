#include "env3.hpp"
#include "modulation_destinations.hpp"
#include "common.hpp"

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

void Env3Model::set_destination(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mDestination, NUM_MODULATION_DESTINATIONS - 1)){
		mDestination = val;
		emit(destination_changed(mDestination));
	}
}

void Env3Model::set_amount(int val){
	if(in_range_and_new<int>(val, mAmount, amount_max, amount_min)){
		mAmount = val;
		emit(amount_changed(mAmount));
	}
}

void Env3Model::set_delay(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mDecay, delay_max)){
		mDelay = val;
		emit(delay_changed(mDelay));
	}
}

void Env3Model::set_attack(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mAttack, attack_max)){
		mAttack = val;
		emit(attack_changed(mAttack));
	}
}

void Env3Model::set_decay(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mDecay, decay_max)){
		mDecay = val;
		emit(decay_changed(mDecay));
	}
}

void Env3Model::set_sustain(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mSustain, sustain_max)){
		mSustain = val;
		emit(sustain_changed(mSustain));
	}
}

void Env3Model::set_release(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mRelease, release_max)){
		mRelease = val;
		emit(release_changed(mRelease));
	}
}

void Env3Model::set_velocity(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mVelocity, velocity_max)){
		mVelocity = val;
		emit(velocity_changed(mVelocity));
	}
}

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include "sliderspinbox.hpp"

Env3View::Env3View(QWidget * parent) : QWidget(parent){
	QLabel * lab;
	//allocate
	mLayout = new QGridLayout(this);

	mDestination = new ModDestComboBox(this);

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
	mLayout->addWidget(mDestination, 7, 1);
	mLayout->addWidget(lab, 7, 0, Qt::AlignRight);

	mLayout->setSpacing(2);
	mLayout->setRowStretch(10, 1);

	setLayout(mLayout);

	//connect out signals
	QObject::connect(
			mDestination,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(destination_changed(int)));
	QObject::connect(
			mDelay,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(delay_changed(int)));
	QObject::connect(
			mAmount,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(amount_changed(int)));
	QObject::connect(
			mAttack,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(attack_changed(int)));
	QObject::connect(
			mDecay,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(decay_changed(int)));
	QObject::connect(
			mSustain,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(sustain_changed(int)));
	QObject::connect(
			mRelease,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(release_changed(int)));
	QObject::connect(
			mVelocity,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(velocity_changed(int)));
	
}

void Env3View::connect_to_model(Env3Model * model){

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

	QObject::connect(
			this,
			SIGNAL(delay_changed(int)),
			model,
			SLOT(set_delay(int)));
	QObject::connect(
			model,
			SIGNAL(delay_changed(int)),
			this,
			SLOT(set_delay(int)));

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
			SIGNAL(attack_changed(int)),
			model,
			SLOT(set_attack(int)));
	QObject::connect(
			model,
			SIGNAL(attack_changed(int)),
			this,
			SLOT(set_attack(int)));

	QObject::connect(
			this,
			SIGNAL(decay_changed(int)),
			model,
			SLOT(set_decay(int)));
	QObject::connect(
			model,
			SIGNAL(decay_changed(int)),
			this,
			SLOT(set_decay(int)));

	QObject::connect(
			this,
			SIGNAL(sustain_changed(int)),
			model,
			SLOT(set_sustain(int)));
	QObject::connect(
			model,
			SIGNAL(sustain_changed(int)),
			this,
			SLOT(set_sustain(int)));

	QObject::connect(
			this,
			SIGNAL(release_changed(int)),
			model,
			SLOT(set_release(int)));
	QObject::connect(
			model,
			SIGNAL(release_changed(int)),
			this,
			SLOT(set_release(int)));

	QObject::connect(
			this,
			SIGNAL(velocity_changed(int)),
			model,
			SLOT(set_velocity(int)));
	QObject::connect(
			model,
			SIGNAL(velocity_changed(int)),
			this,
			SLOT(set_velocity(int)));
}

void Env3View::set_destination(int val){
	mDestination->setCurrentIndex(val);
}

void Env3View::set_delay(int val){
	mDelay->setValue(val);
}

void Env3View::set_amount(int val){
	mAmount->setValue(val);
}

void Env3View::set_attack(int val){
	mAttack->setValue(val);
}

void Env3View::set_decay(int val){
	mDecay->setValue(val);
}

void Env3View::set_sustain(int val){
	mSustain->setValue(val);
}

void Env3View::set_release(int val){
	mRelease->setValue(val);
}

void Env3View::set_velocity(int val){
	mVelocity->setValue(val);
}


