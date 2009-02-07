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

#include "applicationview.hpp"
#include "applicationmodel.hpp"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include "audioandenvelopeview.hpp"
#include "modulationview.hpp"
#include "sequencer.hpp"
#include "mainparameters.hpp"
#include "triggermidi.hpp"

#include "mididriverview.hpp"

QScrollArea * new_scroll_widget(QWidget * obj, QWidget * parent){
	QScrollArea * ret = new QScrollArea(parent);
	ret->setWidget(obj);
	ret->setWidgetResizable(true);
	return ret;
}

ApplicationView::ApplicationView(QWidget * parent) : QWidget(parent){
	//allocate
	mLayout = new QVBoxLayout(this);
	mTabView = new QTabWidget(this);
	mAudioAndEnvelopes = new AudioAndEnvelopeView(this);
	mModulations = new ModulationView(this);
	mSequencer = new SequencerView(this);
	mTriggerMIDI = new TriggerMIDIView(this);

	mMain = new MainView(this);

	mMidiDriver = new MidiDriverView(this);

	//add tabs and layout
	mTabView->addTab(new_scroll_widget(mAudioAndEnvelopes, this), QString("audio and envelopes"));
	mTabView->addTab(new_scroll_widget(mModulations, this), QString("modulations"));
	mTabView->addTab(new_scroll_widget(mSequencer, this), QString("sequencer"));
	mTabView->addTab(new_scroll_widget(mMain, this), QString("main parameters"));
	mTabView->addTab(new_scroll_widget(mTriggerMIDI, this), QString("trigger parameters"));
	mTabView->addTab(mMidiDriver, QString("midi io select"));

	mLayout->addWidget(mTabView);
	mLayout->setContentsMargins(1,1,1,1);
	setLayout(mLayout);
}

AudioAndEnvelopeView * ApplicationView::audio_and_envelopes(){
	return mAudioAndEnvelopes;
}

ModulationView * ApplicationView::modulation(){
	return mModulations;
}

void ApplicationView::connect_to_model(ApplicationModel * model){
	mAudioAndEnvelopes->connect_to_model(model);
	mModulations->connect_to_model(model);
	mSequencer->connect_to_model(model->sequencer());
	mMain->connect_to_model(model->main());
	mTriggerMIDI->connect_to_model(model->trigger_midi());
}

QTabWidget * ApplicationView::tab_widget(){
	return mTabView;
}

MidiDriverView * ApplicationView::midi_driver(){
	return mMidiDriver;
}

MainView * ApplicationView::main(){
	return mMain;
}
