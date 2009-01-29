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

#include "modulators.hpp"
#include "modulation_destinations.hpp"
#include "modulation_sources.hpp"
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include "sliderspinbox.hpp"

const int ModRoutingModel::amount_max = 99;
const int ModRoutingModel::amount_min = -99;

ModRoutingModel::ModRoutingModel(QObject * parent) : QObject(parent){
	mAmount = 0;
	mSource = 0;
	mDestination = 0;
}

int ModRoutingModel::amount(){
	return mAmount;
}

unsigned int ModRoutingModel::source(){
	return mSource;
}

unsigned int ModRoutingModel::destination(){
	return mDestination;
}


void ModRoutingModel::set_amount(int amount){
	if(amount >= amount_min && amount <= amount_max && amount != mAmount){
		mAmount = amount;
		emit(amount_changed(mAmount));
	}
}

void ModRoutingModel::set_source(int source){
	if(source >= 0 &&
			(unsigned int)source < NUM_MODULATION_SOURCES && 
			(unsigned int)source != mSource){
		mSource = source;
		emit(source_changed(mSource));
	}
}

void ModRoutingModel::set_destination(int dest){
	if(dest >= 0 && 
			(unsigned int)dest < NUM_MODULATION_DESTINATIONS && 
			(unsigned int)dest != mDestination){
		mDestination = dest;
		emit(destination_changed(mDestination));
	}
}


ModRoutingView::ModRoutingView(QWidget * parent) : QWidget(parent){
	QLabel * lab;
	mLayout = new QGridLayout(this);

	mAmountSlider = new SliderSpinBox(this);
	mAmountSlider->setRange(ModRoutingModel::amount_min, ModRoutingModel::amount_max);

	mSrcSelect = new QComboBox(this);
	mDestSelect = new ModDestComboBox(this);

	for(unsigned int i = 0; i < NUM_MODULATION_SOURCES; i++)
		mSrcSelect->addItem(QString(modulation_sources[i]));

	lab = new QLabel(QString("source"), this);
	mLabels.push_back(lab);
	mLayout->addWidget(lab, 1, 0, Qt::AlignRight);
	mLayout->addWidget(mSrcSelect, 1, 1);

	lab = new QLabel(QString("amount"), this);
	mLabels.push_back(lab);
	mLayout->addWidget(lab, 2, 0, Qt::AlignRight);
	mLayout->addWidget(mAmountSlider, 2, 1);

	lab = new QLabel(QString("destination"), this);
	mLabels.push_back(lab);
	mLayout->addWidget(lab, 3, 0, Qt::AlignRight);
	mLayout->addWidget(mDestSelect, 3, 1);

	setLayout(mLayout);
	mLayout->setSpacing(1);

	//set out signals
	QObject::connect(mAmountSlider,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(amount_changed(int)));
	QObject::connect(mSrcSelect,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(source_changed(int)));
	QObject::connect(mDestSelect,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(destination_changed(int)));
}

std::vector<QLabel *> * ModRoutingView::labels(){
	return &mLabels;
}

void ModRoutingView::connect_to_model(ModRoutingModel * model){
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
			SIGNAL(source_changed(int)),
			model,
			SLOT(set_source(int)));
	QObject::connect(
			model,
			SIGNAL(source_changed(int)),
			this,
			SLOT(set_source(int)));

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
}

void ModRoutingView::show_labels(bool show){
	for(std::vector<QLabel *>::iterator it = mLabels.begin(); it != mLabels.end(); it++)
		(*it)->setVisible(show);
}

void ModRoutingView::set_amount(int amount){
	mAmountSlider->setValue(amount);
}

void ModRoutingView::set_source(int source){
	mSrcSelect->setCurrentIndex(source);
}

void ModRoutingView::set_destination(int dest){
	mDestSelect->setCurrentIndex(dest);
}


