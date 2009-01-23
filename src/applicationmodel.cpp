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

ApplicationModel::ApplicationModel(QObject * parent) : QObject(parent){
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
	mFeedback = new FilterModel(this);
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

FilterModel * ApplicationModel::feedback(){
	return mFeedback;
}

MiscAudioModel * ApplicationModel::misc_audio(){
	return mMiscAudio;
}

MiscModulationModel * ApplicationModel::misc_modulations(){
	return mMiscModulation;
}

