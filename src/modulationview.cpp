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
#include "modulationview.hpp"
#include "lfo.hpp"
#include "modulators.hpp"
#include "miscmodulation.hpp"
#include "titledwidget.hpp"
#include <QGridLayout>
#include <QLabel>
#include <QSplitter>
#include <QVBoxLayout>

ModulationView::ModulationView(QWidget * parent) : QWidget(parent){
	QVBoxLayout * topLayout = new QVBoxLayout(this);
	QGridLayout * lfoLayout = new QGridLayout;
	QGridLayout * modLayout = new QGridLayout;
	mMiscModulation = new MiscModulationView;

	//allocate the lfos and modulators
	for(unsigned int i = 0; i < 4; i++){
		LFOView * lfo = new LFOView(this);
		lfoLayout->addWidget(new QLabel(QString("lfo %1").arg(i + 1), this), 0, i + 1, Qt::AlignCenter);
		lfoLayout->addWidget(lfo, 1, i + 1);
		mLFOs.push_back(lfo);
		lfo->show_labels(false);

		ModRoutingView * v = new ModRoutingView(this);
		modLayout->addWidget(v, i, 1);
		//modLayout->addWidget(new QLabel(QString("mod %1").arg(i + 1), this), i, 0, Qt::AlignCenter);
		mMods.push_back(v);
	}

	//create the labels
	QVBoxLayout * lLabels = new QVBoxLayout;
	for(std::vector<QLabel *>::iterator it = mLFOs[0]->labels()->begin(); 
			it!= mLFOs[0]->labels()->end(); it++){
		lLabels->addWidget(new QLabel(QString((*it)->text()), this), 0, Qt::AlignRight);
	}
	//add an empty label for the keysync button
	lLabels->addWidget(new QLabel(QString(""), this), 0, Qt::AlignRight);
	lfoLayout->addLayout(lLabels, 1, 0);

	QWidget * lfos = new QWidget(this);
	QWidget * mods = new QWidget;
	lfos->setLayout(lfoLayout);
	mods->setLayout(modLayout);

	QSplitter * bottomSplitter = new QSplitter(Qt::Horizontal, this);
	bottomSplitter->addWidget(new TitledWidget(QString("mod routing"), mods, this));
	bottomSplitter->addWidget(new TitledWidget(QString("misc modulations"), mMiscModulation, this));

	QSplitter * vertSplitter = new QSplitter(Qt::Vertical, this);
	vertSplitter->addWidget(lfos);
	vertSplitter->addWidget(bottomSplitter);

	topLayout->addWidget(vertSplitter);
	topLayout->addStretch(1);

	setLayout(topLayout);
}

void ModulationView::connect_to_model(ApplicationModel * model){
	mMiscModulation->connect_to_model(model->misc_modulations());
	for(unsigned int i = 0; i < 4; i++){
		mLFOs[i]->connect_to_model(model->lfos()->at(i));
		mMods[i]->connect_to_model(model->mods()->at(i));
	}
}

std::vector<LFOView *> * ModulationView::lfos(){
	return &mLFOs;
}

std::vector<ModRoutingView *> * ModulationView::mods(){
	return &mMods;
}

MiscModulationView * ModulationView::misc_modulations(){
	return mMiscModulation;
}

