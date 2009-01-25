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

#include "miscmodulation.hpp"
#include "modulation_destinations.hpp"

const unsigned int MiscModulationModel::num_modulation_sources = 7;
const char * MiscModulationModel::modulation_source_names[] = {
	"input peak",
	"input env follow",
	"midi velocity",
	"midi mod wheel",
	"midi pressure",
	"midi breath",
	"midi foot"
};
const unsigned int MiscModulationModel::osc_slop_max = 5;
const int MiscModulationModel::amount_min = -99;
const int MiscModulationModel::amount_max = 99;

MiscModulationModel::MiscModulationModel(QObject * parent) : Model(parent) {
	mOscSlop = 0;
	for(unsigned int i = 0; i < num_modulation_sources; i++){
		mAmounts.push_back(0);
		mDestinations.push_back(0);
	}
}

void MiscModulationModel::set_osc_slop(int value){
	if(value >= 0 &&
			(unsigned int)value <= osc_slop_max &&
			(unsigned int)value != mOscSlop){
		mOscSlop = value;
		emit(osc_slop_changed(mOscSlop));
		send_program_param(69, mOscSlop);
	}
}

void MiscModulationModel::set_mod_amount(unsigned int index, int value){
	if(index >= num_modulation_sources || value < amount_min || value > amount_max)
		return;
	mAmounts[index] = value;
	emit(mod_amount_changed(index, value));
	send_program_param(114 + (index * 2), value + 99);
}

void MiscModulationModel::set_mod_destination(unsigned int index, int value){
	if(index >= num_modulation_sources || value < 0 || 
			value >= NUM_MODULATION_DESTINATIONS )
		return;
	mDestinations[index] = value;
	emit(mod_destination_changed(index, value));
	send_program_param(115 + (index * 2), value);
}


#include "sliderspinbox.hpp"
#include "modulation_destinations.hpp"
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QSignalMapper>

MiscModulationView::MiscModulationView(QWidget * parent) : QWidget(parent){
	QSignalMapper * amtMapper = new QSignalMapper(this);
	QSignalMapper * destMapper = new QSignalMapper(this);
	QLabel * lab;
	//allocate
	mLayout = new QGridLayout(this);

	mOscSlop = new SliderSpinBox(this);

	for(unsigned int i = 0; i < MiscModulationModel::num_modulation_sources; i++){
		SliderSpinBox * a = new SliderSpinBox(this);
		ModDestComboBox * d = new ModDestComboBox(this);
		mAmounts.push_back(a);
		mDestinations.push_back(d);
		a->setRange(MiscModulationModel::amount_min, MiscModulationModel::amount_max);
		//set up mappings
		QObject::connect(a,
				SIGNAL(valueChanged(int)),
				amtMapper,
				SLOT(map()));
		QObject::connect(d,
				SIGNAL(currentIndexChanged(int)),
				destMapper,
				SLOT(map()));
		amtMapper->setMapping(a, i);
		destMapper->setMapping(d, i);
	}
	//set up mappings
	QObject::connect(amtMapper,
			SIGNAL(mapped(int)),
			this,
			SLOT(update_mod_amount(int)));
	QObject::connect(destMapper,
			SIGNAL(mapped(int)),
			this,
			SLOT(update_mod_dest(int)));

	//setup
	mOscSlop->setRange(0, MiscModulationModel::osc_slop_max);

	//label and plot
	lab = new QLabel(QString("osc slop"));
	mLayout->addWidget(mOscSlop, 0, 1);
	mLayout->addWidget(lab, 0, 0, Qt::AlignRight);

	//all the modulators
	for(unsigned int i = 0; i < MiscModulationModel::num_modulation_sources; i++){

		lab = new QLabel(QString("%1 amt").arg(MiscModulationModel::modulation_source_names[i]));
		mLayout->addWidget(mAmounts[i], 1 + i * 2, 1);
		mLayout->addWidget(lab, 1 + i * 2, 0, Qt::AlignRight);

		lab = new QLabel(QString("%1 dest").arg(MiscModulationModel::modulation_source_names[i]));
		mLayout->addWidget(mDestinations[i], 2 + i * 2, 1);
		mLayout->addWidget(lab, 2 + i * 2, 0, Qt::AlignRight);
	}
	mLayout->setSpacing(2);
	setLayout(mLayout);

	//connect out signals
	QObject::connect(mOscSlop,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(osc_slop_changed(int)));
}

void MiscModulationView::connect_to_model(MiscModulationModel * model){
	QObject::connect(
			this,
			SIGNAL(osc_slop_changed(int)),
			model,
			SLOT(set_osc_slop(int)));
	QObject::connect(
			model,
			SIGNAL(osc_slop_changed(int)),
			this,
			SLOT(set_osc_slop(int)));
	QObject::connect(
			this,
			SIGNAL(mod_amount_changed(unsigned int, int)),
			model,
			SLOT(set_mod_amount(unsigned int, int)));
	QObject::connect(
			model,
			SIGNAL(mod_amount_changed(unsigned int, int)),
			this,
			SLOT(set_mod_amount(unsigned int, int)));
	QObject::connect(
			this,
			SIGNAL(mod_destination_changed(unsigned int, int)),
			model,
			SLOT(set_mod_destination(unsigned int, int)));
	QObject::connect(
			model,
			SIGNAL(mod_destination_changed(unsigned int, int)),
			this,
			SLOT(set_mod_destination(unsigned int, int)));
}

void MiscModulationView::update_mod_amount(int index){
	emit(mod_amount_changed(index, mAmounts[index]->value()));
}

void MiscModulationView::update_mod_dest(int index){
	emit(mod_destination_changed(index, mDestinations[index]->currentIndex()));
}

void MiscModulationView::set_osc_slop(int value){
	mOscSlop->setValue(value);
}

void MiscModulationView::set_mod_amount(unsigned int index, int value){
	if(index < mAmounts.size())
		mAmounts[index]->setValue(value);
}

void MiscModulationView::set_mod_destination(unsigned int index, int value){
	if(index < mDestinations.size())
		mDestinations[index]->setCurrentIndex(value);
}


