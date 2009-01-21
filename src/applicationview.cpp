#include "applicationview.hpp"
#include "oscview.hpp"
#include "lfo.hpp"
#include "modulators.hpp"
#include <QSplitter>
#include <QHBoxLayout>
#include <QLabel>

ApplicationView::ApplicationView(QWidget * parent) : QWidget(parent){
	QGridLayout * topLayout = new QGridLayout(this);
	QGridLayout * oscLayout = new QGridLayout;
	QGridLayout * lfoLayout = new QGridLayout;
	QGridLayout * modLayout = new QGridLayout;

	for(unsigned int i = 0; i < 2; i++){
		DigitalOscView * d = new DigitalOscView(this);
		AnalogOscView * a = new AnalogOscView(this);
		oscLayout->addWidget(a, 1, 1 + i);
		oscLayout->addWidget(d, 3, 1 + i);
		mDigitalOscs.push_back(d);
		mAnalogOscs.push_back(a);
		a->show_labels(false);
		d->show_labels(false);
	}

	for(unsigned int i = 0; i < 4; i++){
		LFOView * lfo = new LFOView(this);
		lfoLayout->addWidget(new QLabel(QString("lfo %1").arg(i + 1), this), 0, i + 1, Qt::AlignCenter);
		lfoLayout->addWidget(lfo, 1, i + 1);
		mLFOs.push_back(lfo);
		lfo->show_labels(false);

		ModView * v = new ModView(this);
		modLayout->addWidget(v, i, 1);
		//modLayout->addWidget(new QLabel(QString("mod %1").arg(i + 1), this), i, 0, Qt::AlignCenter);
		mMods.push_back(v);
	}
	modLayout->setRowStretch(8, 1);

	//create the labels
	QVBoxLayout * aLabels = new QVBoxLayout;
	for(std::vector<QLabel *>::iterator it = mAnalogOscs[0]->labels()->begin(); 
			it!= mAnalogOscs[0]->labels()->end(); it++){
		aLabels->addWidget(new QLabel(QString((*it)->text()), this), 0, Qt::AlignRight);
	}
	oscLayout->addLayout(aLabels, 1, 0, Qt::AlignRight);

	//create the labels
	QVBoxLayout * lLabels = new QVBoxLayout;
	for(std::vector<QLabel *>::iterator it = mLFOs[0]->labels()->begin(); 
			it!= mLFOs[0]->labels()->end(); it++){
		lLabels->addWidget(new QLabel(QString((*it)->text()), this), 0, Qt::AlignRight);
	}
	//add an empty label for the keysync button
	lLabels->addWidget(new QLabel(QString(""), this), 0, Qt::AlignRight);
	lfoLayout->addLayout(lLabels, 1, 0);

	QVBoxLayout * dLabels = new QVBoxLayout;
	for(std::vector<QLabel *>::iterator it = mDigitalOscs[0]->labels()->begin(); 
			it!= mDigitalOscs[0]->labels()->end(); it++){
		dLabels->addWidget(new QLabel(QString((*it)->text()), this), 0, Qt::AlignRight);
	}
	oscLayout->addLayout(dLabels, 3, 0, Qt::AlignRight);

	oscLayout->addWidget(new QLabel("analog osc 1", this), 0, 1, Qt::AlignCenter);
	oscLayout->addWidget(new QLabel("analog osc 2", this), 0, 2, Qt::AlignCenter);
	oscLayout->addWidget(new QLabel("digital osc 1", this), 2, 1, Qt::AlignCenter);
	oscLayout->addWidget(new QLabel("digital osc 2", this), 2, 2, Qt::AlignCenter);

	oscLayout->setRowStretch(10,1);

	QSplitter * topSplitter = new QSplitter(Qt::Horizontal, this);
	QSplitter * leftSplitter = new QSplitter(Qt::Vertical, this);

	QWidget * oscs = new QWidget(this);
	QWidget * lfos = new QWidget(this);
	QWidget * mods = new QWidget(this);
	oscs->setLayout(oscLayout);
	lfos->setLayout(lfoLayout);
	mods->setLayout(modLayout);

	topSplitter->addWidget(oscs);
	topSplitter->addWidget(mods);

	leftSplitter->addWidget(topSplitter);
	leftSplitter->addWidget(lfos);

	topLayout->addWidget(leftSplitter);

	/*
	QSplitter * oscSplitter = new QSplitter(Qt::Vertical, this);

	QWidget * w = new QWidget(this);
	w->setLayout(aLayout);
	oscSplitter->addWidget(w);

	w = new QWidget(this);
	w->setLayout(dLayout);
	oscSplitter->addWidget(w);

	leftSplitter->addWidget(oscSplitter);
	leftSplitter->addWidget(mLFOs);

	topLayout->addWidget(leftSplitter);
	*/
	setLayout(topLayout);
	topLayout->setContentsMargins(1,1,1,1);
}
