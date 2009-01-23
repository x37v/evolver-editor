#include "modulationview.hpp"
#include "lfo.hpp"
#include "modulators.hpp"
#include "miscmodulation.hpp"
#include <QGridLayout>
#include <QLabel>
#include <QSplitter>
#include <QVBoxLayout>

ModulationView::ModulationView(QWidget * parent) : QWidget(parent){
	QVBoxLayout * topLayout = new QVBoxLayout(this);
	QGridLayout * lfoLayout = new QGridLayout;
	QGridLayout * modLayout = new QGridLayout;
	mMiscModulation = new MiscModulationView(this);

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
	QWidget * mods = new QWidget(this);
	lfos->setLayout(lfoLayout);
	mods->setLayout(modLayout);

	QSplitter * bottomSplitter = new QSplitter(Qt::Horizontal, this);
	bottomSplitter->addWidget(mods);
	bottomSplitter->addWidget(mMiscModulation);

	QSplitter * vertSplitter = new QSplitter(Qt::Vertical, this);
	vertSplitter->addWidget(lfos);
	vertSplitter->addWidget(bottomSplitter);

	topLayout->addWidget(vertSplitter);
	topLayout->addStretch(1);

	setLayout(topLayout);
}
