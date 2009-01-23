#include "filter.hpp"

const unsigned int FilterModel::cut_freq_max = 164;
const unsigned int FilterModel::resonance_max = 100;
const unsigned int FilterModel::key_amount_max = 100;
const unsigned int FilterModel::audio_mod_max = 100;
const unsigned int FilterModel::split_max = 100;
const int FilterModel::env_amount_min = -99;
const int FilterModel::env_amount_max = 99;
const unsigned int FilterModel::env_attack_max = 110;
const unsigned int FilterModel::env_decay_max = 110;
const unsigned int FilterModel::env_sustain_max = 100;
const unsigned int FilterModel::env_release_max = 110;
const unsigned int FilterModel::env_velocity_max = 100;
const unsigned int FilterModel::hpf_freq_max = 99;

FilterModel::FilterModel(QObject * parent) : QObject(parent) {
	mMode = two_pole;
	mCutFreq = cut_freq_max;
	mResonance = 0;
	mKeyAmount = 0;
	mAudioMod = 0;
	mSplit = 0;

	mEnvAmt = 0;
	mEnvAttack = 0;
	mEnvDecay = 0;
	mEnvSustain = env_sustain_max;
	mEnvRelease = 110;
	mEnvVelocity = 0;

	mHPFMode = after_lpf;
	mHPFFreq = 0;
}

#include <QPushButton>
#include <QGridLayout>
#include "sliderspinbox.hpp"
#include <QLabel>

FilterView::FilterView(QWidget * parent) : QWidget(parent){
	QLabel * lab;

	//allocate items
	mLayout = new QGridLayout(this);

	mMode = new QPushButton("2 pole", this);
	mCutFreq = new SliderSpinBox(this);
	mResonance = new SliderSpinBox(this);
	mKeyAmount = new SliderSpinBox(this);
	mAudioMod = new SliderSpinBox(this);
	mSplit = new SliderSpinBox(this);

	mEnvAmt = new SliderSpinBox(this);
	mEnvAttack = new SliderSpinBox(this);
	mEnvDecay = new SliderSpinBox(this);
	mEnvSustain = new SliderSpinBox(this);
	mEnvRelease = new SliderSpinBox(this);
	mEnvVelocity = new SliderSpinBox(this);

	mHPFMode = new QPushButton("internal", this);
	mHPFFreq = new SliderSpinBox(this);

	//set ranges
	mCutFreq->setRange(0, FilterModel::cut_freq_max);
	mResonance->setRange(0, FilterModel::resonance_max);
	mKeyAmount->setRange(0, FilterModel::key_amount_max);
	mAudioMod->setRange(0, FilterModel::audio_mod_max);
	mSplit->setRange(0, FilterModel::split_max);

	mEnvAmt->setRange(FilterModel::env_amount_min, FilterModel::env_amount_max);
	mEnvAttack->setRange(0, FilterModel::env_attack_max);
	mEnvDecay->setRange(0, FilterModel::env_decay_max);
	mEnvSustain->setRange(0, FilterModel::env_sustain_max);
	mEnvRelease->setRange(0, FilterModel::env_release_max);
	mEnvVelocity->setRange(0, FilterModel::env_velocity_max);

	mHPFFreq->setRange(0, FilterModel::hpf_freq_max);

	//put them in our layout
	lab = new QLabel(QString("pole select"));
	mLayout->addWidget(mMode, 0, 1);
	mLayout->addWidget(lab, 0, 0, Qt::AlignRight);

	lab = new QLabel(QString("cut freq"));
	mLayout->addWidget(mCutFreq, 1, 1);
	mLayout->addWidget(lab, 1, 0, Qt::AlignRight);

	lab = new QLabel(QString("resonance"));
	mLayout->addWidget(mResonance, 2, 1);
	mLayout->addWidget(lab, 2, 0, Qt::AlignRight);

	lab = new QLabel(QString("key amt"));
	mLayout->addWidget(mKeyAmount, 3, 1);
	mLayout->addWidget(lab, 3, 0, Qt::AlignRight);

	lab = new QLabel(QString("audio mod"));
	mLayout->addWidget(mAudioMod, 4, 1);
	mLayout->addWidget(lab, 4, 0, Qt::AlignRight);

	lab = new QLabel(QString("split"));
	mLayout->addWidget(mSplit, 5, 1);
	mLayout->addWidget(lab, 5, 0, Qt::AlignRight);


	lab = new QLabel(QString("envelope"));
	mLayout->addWidget(lab, 6, 1, 1, 2, Qt::AlignCenter);

	lab = new QLabel(QString("amount"));
	mLayout->addWidget(mEnvAmt, 7, 1);
	mLayout->addWidget(lab, 7, 0, Qt::AlignRight);

	lab = new QLabel(QString("attack"));
	mLayout->addWidget(mEnvAttack, 8, 1);
	mLayout->addWidget(lab, 8, 0, Qt::AlignRight);

	lab = new QLabel(QString("decay"));
	mLayout->addWidget(mEnvDecay, 9, 1);
	mLayout->addWidget(lab, 9, 0, Qt::AlignRight);

	lab = new QLabel(QString("sustain"));
	mLayout->addWidget(mEnvSustain, 10, 1);
	mLayout->addWidget(lab, 10, 0, Qt::AlignRight);

	lab = new QLabel(QString("release"));
	mLayout->addWidget(mEnvRelease, 11, 1);
	mLayout->addWidget(lab, 11, 0, Qt::AlignRight);

	lab = new QLabel(QString("velocity"));
	mLayout->addWidget(mEnvVelocity, 12, 1);
	mLayout->addWidget(lab, 12, 0, Qt::AlignRight);


	lab = new QLabel(QString("hpf"));
	mLayout->addWidget(lab, 13, 1, 1, 2, Qt::AlignCenter);

	lab = new QLabel(QString("location"));
	mLayout->addWidget(mHPFMode, 14, 1);
	mLayout->addWidget(lab, 14, 0, Qt::AlignRight);

	lab = new QLabel(QString("freq"));
	mLayout->addWidget(mHPFFreq, 15, 1);
	mLayout->addWidget(lab, 15, 0, Qt::AlignRight);

	mLayout->setRowStretch(16, 1);

	setLayout(mLayout);
	mLayout->setSpacing(2);
}

