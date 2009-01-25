#include "applicationmodel.hpp"
#include "lfo.hpp"
#include "oscmodel.hpp"
#include "modulators.hpp"
#include "filter.hpp"
#include "vca.hpp"
#include "env3.hpp"
#include "delay.hpp"
#include "feedback.hpp"
#include "miscaudio.hpp"
#include "miscmodulation.hpp"
#include <QSignalMapper>

ApplicationModel::ApplicationModel(QObject * parent) : Model(parent){
	QSignalMapper * oscFreqMap = new QSignalMapper(this);
	QSignalMapper * oscTuneMap = new QSignalMapper(this);
	QSignalMapper * oscGlideMap = new QSignalMapper(this);
	QSignalMapper * oscLevelMap = new QSignalMapper(this);
	QSignalMapper * oscGlideMode = new QSignalMapper(this);

	QSignalMapper * aOscShapeMap = new QSignalMapper(this);
	QSignalMapper * aOscSyncMap = new QSignalMapper(this);
	//allocate
	for(unsigned int i = 0; i < 2; i++){
		DigitalOscModel * d = new DigitalOscModel(this);
		AnalogOscModel * a = new AnalogOscModel(this);
		mDigitalOscs.push_back(d);
		mAnalogOscs.push_back(a);
		QObject::connect(a, SIGNAL(freq_changed(int)), oscFreqMap, SLOT(map()));
		QObject::connect(d, SIGNAL(freq_changed(int)), oscFreqMap, SLOT(map()));
		QObject::connect(a, SIGNAL(tune_changed(int)), oscTuneMap, SLOT(map()));
		QObject::connect(d, SIGNAL(tune_changed(int)), oscTuneMap, SLOT(map()));
		QObject::connect(a, SIGNAL(glide_changed(int)), oscGlideMap, SLOT(map()));
		QObject::connect(d, SIGNAL(glide_changed(int)), oscGlideMap, SLOT(map()));
		QObject::connect(a, SIGNAL(level_changed(int)), oscLevelMap, SLOT(map()));
		QObject::connect(d, SIGNAL(level_changed(int)), oscLevelMap, SLOT(map()));
		QObject::connect(a, SIGNAL(glide_mode_changed(int)), oscGlideMode, SLOT(map()));
		QObject::connect(d, SIGNAL(glide_mode_changed(int)), oscGlideMode, SLOT(map()));
		oscFreqMap->setMapping(a, i);
		oscFreqMap->setMapping(d, i + 2);
		oscTuneMap->setMapping(a, i);
		oscTuneMap->setMapping(d, i + 2);
		oscGlideMap->setMapping(a, i);
		oscGlideMap->setMapping(d, i + 2);
		oscLevelMap->setMapping(a, i);
		oscLevelMap->setMapping(d, i + 2);
		oscGlideMode->setMapping(a, i);
		oscGlideMode->setMapping(d, i + 2);

		//shape and width do the same thing really, so just use one map
		QObject::connect(a, SIGNAL(shape_changed(int)), aOscShapeMap, SLOT(map()));
		QObject::connect(a, SIGNAL(width_changed(int)), aOscShapeMap, SLOT(map()));
		QObject::connect(a, SIGNAL(sync_changed(bool)), aOscSyncMap, SLOT(map()));
		aOscShapeMap->setMapping(a, i);
		aOscSyncMap->setMapping(a, i);
	}
	QObject::connect(oscFreqMap, SIGNAL(mapped(int)), this, SLOT(osc_set_freq(int)));
	QObject::connect(oscTuneMap, SIGNAL(mapped(int)), this, SLOT(osc_set_tune(int)));
	QObject::connect(oscGlideMap, SIGNAL(mapped(int)), this, SLOT(osc_set_glide(int)));
	QObject::connect(oscLevelMap, SIGNAL(mapped(int)), this, SLOT(osc_set_level(int)));
	QObject::connect(oscGlideMode, SIGNAL(mapped(int)), this, SLOT(osc_set_glide_mode(int)));

	QObject::connect(aOscShapeMap, SIGNAL(mapped(int)), this, SLOT(analog_osc_set_shape(int)));
	QObject::connect(aOscSyncMap, SIGNAL(mapped(int)), this, SLOT(analog_osc_set_sync(int)));
	for(unsigned int i = 0; i < 4; i++){
		mLFOs.push_back(new LFOModel(this));
		mMods.push_back(new ModRoutingModel(this));
	}
	mFilter = new FilterModel(this);
	mVCA = new VCAModel(this);
	mEnv3 = new Env3Model(this);
	mDelay = new DelayModel(this);
	mFeedback = new FeedbackModel(this);
	mMiscAudio = new MiscAudioModel(this);
	mMiscModulation = new MiscModulationModel(this);
}

std::vector<AnalogOscModel *> * ApplicationModel::analog_oscs(){
	return &mAnalogOscs;
}

std::vector<DigitalOscModel *> * ApplicationModel::digital_oscs(){
	return &mDigitalOscs;
}

std::vector<LFOModel *> * ApplicationModel::lfos(){
	return &mLFOs;
}

std::vector<ModRoutingModel *> * ApplicationModel::mods(){
	return &mMods;
}

FilterModel * ApplicationModel::filter(){
	return mFilter;
}

VCAModel * ApplicationModel::vca(){
	return mVCA;
}

Env3Model * ApplicationModel::env3(){
	return mEnv3;
}

DelayModel * ApplicationModel::delay(){
	return mDelay;
}

FeedbackModel * ApplicationModel::feedback(){
	return mFeedback;
}

MiscAudioModel * ApplicationModel::misc_audio(){
	return mMiscAudio;
}

MiscModulationModel * ApplicationModel::misc_modulations(){
	return mMiscModulation;
}


void ApplicationModel::osc_set_freq(int index){
	if(index < 2)
		send_program_param(index * 4, mAnalogOscs[index]->freq());
	else
		send_program_param(index * 4, mDigitalOscs[index - 2]->freq());
}

void ApplicationModel::osc_set_tune(int index){
	if(index < 2)
		send_program_param(1 + index * 4, mAnalogOscs[index]->tune() + 50);
	else
		send_program_param(1 + index * 4, mDigitalOscs[index - 2]->tune() + 50);
}

void ApplicationModel::osc_set_glide(int index){
	if (index < 2){
		switch(mAnalogOscs[index]->glide_mode()){
			case OscModel::normal:
				send_program_param(64 + 4 * index, mAnalogOscs[index]->glide());
				break;
			case OscModel::fingered:
				send_program_param(64 + 4 * index, 99 + mAnalogOscs[index]->glide());
				break;
			case OscModel::off:
				send_program_param(64 + 4 * index, 200);
				break;
		};
	} else {
		switch(mDigitalOscs[(index - 2)]->glide_mode()){
			case OscModel::normal:
				send_program_param(72 + 4 * (index - 2), mDigitalOscs[(index - 2)]->glide());
				break;
			case OscModel::fingered:
				send_program_param(72 + 4 * (index - 2), 99 + mDigitalOscs[(index - 2)]->glide());
				break;
			case OscModel::off:
				send_program_param(72 + 4 * (index - 2), 200);
				break;
		};
	}
}

void ApplicationModel::osc_set_level(int index){
	if(index < 2)
		send_program_param(3 + index * 4, mAnalogOscs[index]->level());
	else
		send_program_param(3 + index * 4, mDigitalOscs[index - 2]->level());
}

void ApplicationModel::osc_set_glide_mode(int index){
	if (index < 2){
		switch(mAnalogOscs[index]->glide_mode()){
			case OscModel::normal:
				send_program_param(64 + 4 * index, mAnalogOscs[index]->glide());
				break;
			case OscModel::fingered:
				send_program_param(64 + 4 * index, 99 + mAnalogOscs[index]->glide());
				break;
			case OscModel::off:
				send_program_param(64 + 4 * index, 200);
				break;
		};
	} else {
		switch(mDigitalOscs[(index - 2)]->glide_mode()){
			case OscModel::normal:
				send_program_param(72 + 4 * (index - 2), mDigitalOscs[(index - 2)]->glide());
				break;
			case OscModel::fingered:
				send_program_param(72 + 4 * (index - 2), 99 + mDigitalOscs[(index - 2)]->glide());
				break;
			case OscModel::off:
				send_program_param(72 + 4 * (index - 2), 200);
				break;
		};
	}
}

void ApplicationModel::analog_osc_set_shape(int index){
	if(mAnalogOscs[index]->shape() == AnalogOscModel::pulse)
		send_program_param(2 + 4 * index, AnalogOscModel::pulse + mAnalogOscs[index]->width());
	else
		send_program_param(2 + 4 * index, mAnalogOscs[index]->shape());
}

void ApplicationModel::analog_osc_set_width(int index){
	if(mAnalogOscs[index]->shape() == AnalogOscModel::pulse)
		send_program_param(2 + 4 * index, AnalogOscModel::pulse + mAnalogOscs[index]->width());
}

void ApplicationModel::analog_osc_set_sync(int index){
	if(index == 0)
		send_program_param(65, mAnalogOscs[0]->sync());
}

void ApplicationModel::digital_osc_set_shape(int index){
	send_program_param(10 + 4 * index, mDigitalOscs[index]->shape());
}

void ApplicationModel::digital_osc_set_fm_in(int index){
	send_program_param(73 + 4 * index, mDigitalOscs[index]->fm_in());
}

void ApplicationModel::digital_osc_set_ring_in(int index){
	send_program_param(75 + 4 * index, mDigitalOscs[index]->ring_in());
}

void ApplicationModel::digital_osc_set_shape_mod(int index){
	send_program_param(74 + 4 * index, mDigitalOscs[index]->shape_mod());
}


void ApplicationModel::lfo_set_freq(int index){
}

void ApplicationModel::lfo_set_amount(int index){
}

void ApplicationModel::lfo_set_shape(int index){
}

void ApplicationModel::lfo_set_key_sync(int index){
}

void ApplicationModel::lfo_set_sync_type(int index){
}

void ApplicationModel::lfo_set_destination(int index){
}


void ApplicationModel::mod_set_amount(int index){
}

void ApplicationModel::mod_set_source(int index){
}

void ApplicationModel::mod_set_destination(int index){
}

