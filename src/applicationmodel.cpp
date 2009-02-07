/*
 *		Copyright (c) 2009 Alex Norman.  All rights reserved.
 *		http://www.x37v.info/
 *
 *		This file is part of Alex's Evolver Editor.
 *		
 *		This Evolver Editor is free software: you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License as published
 *		by the Free Software Foundation, either version 3 of the License, or (at
 *		your option) any later version.
 *		
 *		This Evolver Editor is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 *		Public License for more details.
 *		
 *		You should have received a copy of the GNU General Public License along
 *		with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

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
#include "sequencer.hpp"
#include "triggermidi.hpp"
#include "mainparameters.hpp"
#include <QSignalMapper>

ApplicationModel::ApplicationModel(QObject * parent) : Model(parent){
	QSignalMapper * oscFreqMap = new QSignalMapper(this);
	QSignalMapper * oscTuneMap = new QSignalMapper(this);
	QSignalMapper * oscGlideMap = new QSignalMapper(this);
	QSignalMapper * oscLevelMap = new QSignalMapper(this);
	QSignalMapper * oscGlideMode = new QSignalMapper(this);

	QSignalMapper * aOscShapeMap = new QSignalMapper(this);
	QSignalMapper * aOscSyncMap = new QSignalMapper(this);

	QSignalMapper * dOscShapeMap = new QSignalMapper(this);
	QSignalMapper * dOscFmInMap = new QSignalMapper(this);
	QSignalMapper * dOscRingInMap = new QSignalMapper(this);
	QSignalMapper * dOscShapeModMap = new QSignalMapper(this);
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

		QObject::connect(d, SIGNAL(shape_changed(int)), dOscShapeMap, SLOT(map()));
		QObject::connect(d, SIGNAL(fm_in_changed(int)), dOscFmInMap, SLOT(map()));
		QObject::connect(d, SIGNAL(ring_in_changed(int)), dOscRingInMap, SLOT(map()));
		QObject::connect(d, SIGNAL(shape_mod_changed(int)), dOscShapeModMap, SLOT(map()));
		dOscShapeMap->setMapping(d, i);
		dOscFmInMap->setMapping(d, i);
		dOscRingInMap->setMapping(d, i);
		dOscShapeModMap->setMapping(d, i);
	}
	QObject::connect(oscFreqMap, SIGNAL(mapped(int)), this, SLOT(osc_set_freq(int)));
	QObject::connect(oscTuneMap, SIGNAL(mapped(int)), this, SLOT(osc_set_tune(int)));
	QObject::connect(oscGlideMap, SIGNAL(mapped(int)), this, SLOT(osc_set_glide(int)));
	QObject::connect(oscLevelMap, SIGNAL(mapped(int)), this, SLOT(osc_set_level(int)));
	QObject::connect(oscGlideMode, SIGNAL(mapped(int)), this, SLOT(osc_set_glide_mode(int)));

	QObject::connect(aOscShapeMap, SIGNAL(mapped(int)), this, SLOT(analog_osc_set_shape(int)));
	QObject::connect(aOscSyncMap, SIGNAL(mapped(int)), this, SLOT(analog_osc_set_sync(int)));

	QObject::connect(dOscShapeMap, SIGNAL(mapped(int)), this, SLOT(digital_osc_set_shape(int)));
	QObject::connect(dOscFmInMap, SIGNAL(mapped(int)), this, SLOT(digital_osc_set_fm_in(int)));
	QObject::connect(dOscRingInMap, SIGNAL(mapped(int)), this, SLOT(digital_osc_set_ring_in(int)));
	QObject::connect(dOscShapeModMap, SIGNAL(mapped(int)), this, SLOT(digital_osc_set_shape_mod(int)));

	//key sync maps to amount, sync_type maps to freq
	QSignalMapper * lfoFreqMap = new QSignalMapper(this);
	QSignalMapper * lfoAmountMap = new QSignalMapper(this);
	QSignalMapper * lfoShapeMap = new QSignalMapper(this);
	QSignalMapper * lfoDestMap = new QSignalMapper(this);

	QSignalMapper * modDestMap = new QSignalMapper(this);
	QSignalMapper * modSrcMap = new QSignalMapper(this);
	QSignalMapper * modAmtMap = new QSignalMapper(this);

	for(unsigned int i = 0; i < 4; i++){
		LFOModel * lfo = new LFOModel(this);
		ModRoutingModel * mod = new ModRoutingModel(this);
		mLFOs.push_back(lfo);
		mMods.push_back(mod);

		QObject::connect(lfo, SIGNAL(freq_changed(int)), lfoFreqMap, SLOT(map()));
		QObject::connect(lfo, SIGNAL(sync_type_changed(int)), lfoFreqMap, SLOT(map()));
		QObject::connect(lfo, SIGNAL(amount_changed(int)), lfoAmountMap, SLOT(map()));
		QObject::connect(lfo, SIGNAL(key_sync_changed(bool)), lfoAmountMap, SLOT(map()));
		QObject::connect(lfo, SIGNAL(shape_changed(int)), lfoShapeMap, SLOT(map()));
		QObject::connect(lfo, SIGNAL(destination_changed(int)), lfoDestMap, SLOT(map()));

		QObject::connect(mod, SIGNAL(amount_changed(int)), modAmtMap, SLOT(map()));
		QObject::connect(mod, SIGNAL(source_changed(int)), modSrcMap, SLOT(map()));
		QObject::connect(mod, SIGNAL(destination_changed(int)), modDestMap, SLOT(map()));

		lfoFreqMap->setMapping(lfo, i);
		lfoAmountMap->setMapping(lfo, i);
		lfoShapeMap->setMapping(lfo, i);
		lfoDestMap->setMapping(lfo, i);

		modAmtMap->setMapping(mod, i);
		modSrcMap->setMapping(mod, i);
		modDestMap->setMapping(mod, i);
	}
	QObject::connect(lfoFreqMap, SIGNAL(mapped(int)), this, SLOT(lfo_set_freq(int)));
	QObject::connect(lfoAmountMap, SIGNAL(mapped(int)), this, SLOT(lfo_set_amount(int)));
	QObject::connect(lfoShapeMap, SIGNAL(mapped(int)), this, SLOT(lfo_set_shape(int)));
	QObject::connect(lfoDestMap, SIGNAL(mapped(int)), this, SLOT(lfo_set_destination(int)));

	QObject::connect(modAmtMap, SIGNAL(mapped(int)), this, SLOT(mod_set_amount(int)));
	QObject::connect(modSrcMap, SIGNAL(mapped(int)), this, SLOT(mod_set_source(int)));
	QObject::connect(modDestMap, SIGNAL(mapped(int)), this, SLOT(mod_set_destination(int)));

	mFilter = new FilterModel(this);
	mVCA = new VCAModel(this);
	mEnv3 = new Env3Model(this);
	mDelay = new DelayModel(this);
	mFeedback = new FeedbackModel(this);
	mMiscAudio = new MiscAudioModel(this);
	mMiscModulation = new MiscModulationModel(this);
	mSequencer = new SequencerModel(this);
	mTriggerMIDI = new TriggerMIDIModel(this);
	mMain = new MainModel(this);
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

SequencerModel * ApplicationModel::sequencer(){
	return mSequencer;
}

TriggerMIDIModel * ApplicationModel::trigger_midi(){
	return mTriggerMIDI;
}

MainModel * ApplicationModel::main(){
	return mMain;
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
	send_program_param(10 + 4 * index, mDigitalOscs[index]->shape() - 1);
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
	int param_num = 40;
	if(index == 1)
		param_num = 44;
	else if(index == 2)
		param_num = 104;
	else if(index == 3)
		param_num = 108;

	if(mLFOs[index]->sync_type() == LFOModel::off)
		send_program_param(param_num, mLFOs[index]->freq());
	else
		send_program_param(param_num, mLFOs[index]->sync_type() + 150);
}

void ApplicationModel::lfo_set_amount(int index){
	int param_num = 42;
	if(index == 1)
		param_num = 46;
	else if(index == 2)
		param_num = 106;
	else if(index == 3)
		param_num = 110;

	if(mLFOs[index]->key_sync())
		send_program_param(param_num, mLFOs[index]->amount() + 100);
	else
		send_program_param(param_num, mLFOs[index]->amount());
}

void ApplicationModel::lfo_set_shape(int index){
	switch(index){
		case 0:
			send_program_param(41, mLFOs[index]->shape());
			break;
		case 1:
			send_program_param(45, mLFOs[index]->shape());
			break;
		case 2:
			send_program_param(105, mLFOs[index]->shape());
			break;
		case 3:
			send_program_param(109, mLFOs[index]->shape());
			break;
		default:
			break;
	};
}

//void ApplicationModel::lfo_set_key_sync(int index){
//}

//void ApplicationModel::lfo_set_sync_type(int index){
//}

void ApplicationModel::lfo_set_destination(int index){
	switch(index){
		case 0:
			send_program_param(43, mLFOs[index]->destination());
			break;
		case 1:
			send_program_param(47, mLFOs[index]->destination());
			break;
		case 2:
			send_program_param(107, mLFOs[index]->destination());
			break;
		case 3:
			send_program_param(111, mLFOs[index]->destination());
			break;
		default:
			break;
	};
}


void ApplicationModel::mod_set_amount(int index){
	if(index == 0)
		send_program_param(86, mMods[index]->amount() + 99);
	else
		send_program_param(91 + 3 * (index - 1), mMods[index]->amount() + 99);
}

void ApplicationModel::mod_set_source(int index){
	if(index == 0)
		send_program_param(85, mMods[index]->source());
	else
		send_program_param(90 + 3 * (index - 1), mMods[index]->source());
}

void ApplicationModel::mod_set_destination(int index){
	if(index == 0)
		send_program_param(87, mMods[index]->destination());
	else
		send_program_param(92 + 3 * (index - 1), mMods[index]->destination());
}

