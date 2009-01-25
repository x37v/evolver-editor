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

ApplicationModel::ApplicationModel(QObject * parent) : Model(parent){
	//allocate
	for(unsigned int i = 0; i < 2; i++){
		mDigitalOscs.push_back(new DigitalOscModel(this));
		mAnalogOscs.push_back(new AnalogOscModel(this));
	}
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
}

void ApplicationModel::osc_set_tune(int index){
}

void ApplicationModel::osc_set_glide(int index){
}

void ApplicationModel::osc_set_level(int index){
}

void ApplicationModel::osc_set_glide_mode(int index){
}

void ApplicationModel::analog_osc_set_shape(int index){
}

void ApplicationModel::analog_osc_set_width(int index){
}

void ApplicationModel::analog_osc_set_sync(int index){
}

void ApplicationModel::digital_osc_set_shape(int index){
}

void ApplicationModel::digital_osc_set_fm_in(int index){
}

void ApplicationModel::digital_osc_set_ring_in(int index){
}

void ApplicationModel::digital_osc_set_shape_mod(int index){
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

