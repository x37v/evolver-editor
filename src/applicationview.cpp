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

#include "mididriverview.hpp"

ApplicationView::ApplicationView(QWidget * parent) : QWidget(parent){
	//allocate
	mLayout = new QVBoxLayout(this);
	mTabView = new QTabWidget(this);
	mAudioAndEnvelopes = new AudioAndEnvelopeView(this);
	mModulations = new ModulationView(this);
	mSequencer = new SequencerView(this);

	mMidiDriver = new MidiDriverView(this);

	//make them scrollable
	QScrollArea * tab0 = new QScrollArea(this);
	QScrollArea * tab1 = new QScrollArea(this);
	QScrollArea * tab2 = new QScrollArea(this);
	tab0->setWidget(mAudioAndEnvelopes);
	tab1->setWidget(mModulations);
	tab2->setWidget(mSequencer);

	//let the tabs resize
	tab0->setWidgetResizable(true);
	tab1->setWidgetResizable(true);
	tab2->setWidgetResizable(true);

	//add tabs and layout
	mTabView->addTab(tab0, QString("audio and envelopes"));
	mTabView->addTab(tab1, QString("modulations"));
	mTabView->addTab(tab2, QString("sequencer"));
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
}

QTabWidget * ApplicationView::tab_widget(){
	return mTabView;
}

MidiDriverView * ApplicationView::midi_driver(){
	return mMidiDriver;
}
