#include "filter.hpp"
#include "common.hpp"

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

void FilterModel::set_mode(int val){
	if(val < 0 or val > 1)
		return;
	if(val != mMode){
		mMode = (lpf_mode)val;
		emit(mode_changed(mMode));
	}
}

void FilterModel::set_cutoff(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mCutFreq, cut_freq_max)){
		mCutFreq = val;
		emit(cutoff_changed(mCutFreq));
	}
}

void FilterModel::set_resonance(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mResonance, resonance_max)){
		mResonance = val;
		emit(resonance_changed(mResonance));
	}
}

void FilterModel::set_key_amount(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mKeyAmount, key_amount_max)){
		mKeyAmount = val;
		emit(key_amount_changed(mKeyAmount));
	}
}

void FilterModel::set_audio_mod(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mAudioMod, audio_mod_max)){
		mAudioMod = val;
		emit(audio_mod_changed(mAudioMod));
	}
}

void FilterModel::set_split(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mSplit, split_max)){
		mSplit = val;
		emit(split_changed(mSplit));
	}
}

void FilterModel::set_env_amount(int val){
	if(in_range_and_new<int>(val, mEnvAmt, env_amount_max, env_amount_min)){
		mEnvAmt = val;
		emit(env_amount_changed(mEnvAmt));
	}
}

void FilterModel::set_env_attack(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mEnvAttack, env_attack_max)){
		mEnvAttack = val;
		emit(env_attack_changed(mEnvAttack));
	}
}

void FilterModel::set_env_decay(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mEnvDecay, env_decay_max)){
		mEnvDecay = val;
		emit(env_decay_changed(mEnvDecay));
	}
}

void FilterModel::set_env_sustain(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mEnvSustain, env_sustain_max)){
		mEnvSustain = val;
		emit(env_sustain_changed(mEnvSustain));
	}
}

void FilterModel::set_env_release(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mEnvRelease, env_release_max)){
		mEnvRelease = val;
		emit(env_release_changed(mEnvRelease));
	}
}

void FilterModel::set_env_velocity(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mEnvVelocity, env_velocity_max)){
		mEnvVelocity = val;
		emit(env_velocity_changed(mEnvVelocity));
	}
}

void FilterModel::set_hpf_mode(int val){
	if(val < 0 || val > 1)
		return;
	if(val != mHPFMode){
		mHPFMode = (hpf_mode)val;
		emit(hpf_mode_changed(mHPFMode));
	}
}

void FilterModel::set_hpf_cutoff(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mHPFFreq, hpf_freq_max)){
		mHPFFreq = val;
		emit(hpf_cutoff_changed(mHPFFreq));
	}
}


#include <QComboBox>
#include <QGridLayout>
#include "sliderspinbox.hpp"
#include <QLabel>

FilterView::FilterView(QWidget * parent) : QWidget(parent){
	QLabel * lab;

	//allocate items
	mLayout = new QGridLayout(this);

	mMode = new QComboBox(this);
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

	mHPFMode = new QComboBox(this);
	mHPFFreq = new SliderSpinBox(this);

	//set ranges
	mMode->addItem("2 pole");
	mMode->addItem("4 pole");
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

	mHPFMode->addItem("internal");
	mHPFMode->addItem("external");
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

	//connect out signals
	QObject::connect(mMode,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(mode_changed(int)));
	QObject::connect(mCutFreq,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(cutoff_changed(int)));
	QObject::connect(mResonance,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(resonance_changed(int)));
	QObject::connect(mKeyAmount,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(key_amount_changed(int)));
	QObject::connect(mAudioMod,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(audio_mod_changed(int)));
	QObject::connect(mSplit,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(split_changed(int)));
	QObject::connect(mEnvAmt,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(env_amount_changed(int)));
	QObject::connect(mEnvAttack,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(env_attack_changed(int)));
	QObject::connect(mEnvDecay,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(env_decay_changed(int)));
	QObject::connect(mEnvSustain,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(env_sustain_changed(int)));
	QObject::connect(mEnvRelease,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(env_release_changed(int)));
	QObject::connect(mEnvVelocity,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(env_velocity_changed(int)));
	QObject::connect(mHPFMode,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(hpf_mode_changed(int)));
	QObject::connect(mHPFFreq,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(hpf_cutoff_changed(int)));
}

void FilterView::connect_to_model(FilterModel * model){
	QObject::connect(
			this,
			SIGNAL(mode_changed(int)),
			model,
			SLOT(set_mode(int)));
	QObject::connect(
			model,
			SIGNAL(mode_changed(int)),
			this,
			SLOT(set_mode(int)));
	QObject::connect(
			this,
			SIGNAL(cutoff_changed(int)),
			model,
			SLOT(set_cutoff(int)));
	QObject::connect(
			model,
			SIGNAL(cutoff_changed(int)),
			this,
			SLOT(set_cutoff(int)));
	QObject::connect(
			this,
			SIGNAL(resonance_changed(int)),
			model,
			SLOT(set_resonance(int)));
	QObject::connect(
			model,
			SIGNAL(resonance_changed(int)),
			this,
			SLOT(set_resonance(int)));
	QObject::connect(
			this,
			SIGNAL(key_amount_changed(int)),
			model,
			SLOT(set_key_amount(int)));
	QObject::connect(
			model,
			SIGNAL(key_amount_changed(int)),
			this,
			SLOT(set_key_amount(int)));
	QObject::connect(
			this,
			SIGNAL(audio_mod_changed(int)),
			model,
			SLOT(set_audio_mod(int)));
	QObject::connect(
			model,
			SIGNAL(audio_mod_changed(int)),
			this,
			SLOT(set_audio_mod(int)));
	QObject::connect(
			this,
			SIGNAL(split_changed(int)),
			model,
			SLOT(set_split(int)));
	QObject::connect(
			model,
			SIGNAL(split_changed(int)),
			this,
			SLOT(set_split(int)));
	QObject::connect(
			this,
			SIGNAL(env_amount_changed(int)),
			model,
			SLOT(set_env_amount(int)));
	QObject::connect(
			model,
			SIGNAL(env_amount_changed(int)),
			this,
			SLOT(set_env_amount(int)));
	QObject::connect(
			this,
			SIGNAL(env_attack_changed(int)),
			model,
			SLOT(set_env_attack(int)));
	QObject::connect(
			model,
			SIGNAL(env_attack_changed(int)),
			this,
			SLOT(set_env_attack(int)));
	QObject::connect(
			this,
			SIGNAL(env_decay_changed(int)),
			model,
			SLOT(set_env_decay(int)));
	QObject::connect(
			model,
			SIGNAL(env_decay_changed(int)),
			this,
			SLOT(set_env_decay(int)));
	QObject::connect(
			this,
			SIGNAL(env_sustain_changed(int)),
			model,
			SLOT(set_env_sustain(int)));
	QObject::connect(
			model,
			SIGNAL(env_sustain_changed(int)),
			this,
			SLOT(set_env_sustain(int)));
	QObject::connect(
			this,
			SIGNAL(env_release_changed(int)),
			model,
			SLOT(set_env_release(int)));
	QObject::connect(
			model,
			SIGNAL(env_release_changed(int)),
			this,
			SLOT(set_env_release(int)));
	QObject::connect(
			this,
			SIGNAL(env_velocity_changed(int)),
			model,
			SLOT(set_env_velocity(int)));
	QObject::connect(
			model,
			SIGNAL(env_velocity_changed(int)),
			this,
			SLOT(set_env_velocity(int)));
	QObject::connect(
			this,
			SIGNAL(hpf_mode_changed(int)),
			model,
			SLOT(set_hpf_mode(int)));
	QObject::connect(
			model,
			SIGNAL(hpf_mode_changed(int)),
			this,
			SLOT(set_hpf_mode(int)));
	QObject::connect(
			this,
			SIGNAL(hpf_cutoff_changed(int)),
			model,
			SLOT(set_hpf_cutoff(int)));
	QObject::connect(
			model,
			SIGNAL(hpf_cutoff_changed(int)),
			this,
			SLOT(set_hpf_cutoff(int)));
}

void FilterView::set_mode(int val){
	mMode->setCurrentIndex(val);
}

void FilterView::set_cutoff(int val){
	mCutFreq->setValue(val);
}

void FilterView::set_resonance(int val){
	mResonance->setValue(val);
}

void FilterView::set_key_amount(int val){
	mKeyAmount->setValue(val);
}

void FilterView::set_audio_mod(int val){
	mAudioMod->setValue(val);
}

void FilterView::set_split(int val){
	mSplit->setValue(val);
}

void FilterView::set_env_amount(int val){
	mEnvAmt->setValue(val);
}

void FilterView::set_env_attack(int val){
	mEnvAttack->setValue(val);
}

void FilterView::set_env_decay(int val){
	mEnvDecay->setValue(val);
}

void FilterView::set_env_sustain(int val){
	mEnvSustain->setValue(val);
}

void FilterView::set_env_release(int val){
	mEnvRelease->setValue(val);
}

void FilterView::set_env_velocity(int val){
	mEnvVelocity->setValue(val);
}

void FilterView::set_hpf_mode(int val){
	mHPFMode->setCurrentIndex(val);
}

void FilterView::set_hpf_cutoff(int val){
	mHPFFreq->setValue(val);
}

