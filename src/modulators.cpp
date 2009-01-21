#include "modulators.hpp"
#include "modulation_destinations.hpp"
#include "modulation_sources.hpp"
#include <QLabel>
#include <QComboBox>
#include <QGridLayout>
#include "sliderspinbox.hpp"

const int ModModel::amount_max = 99;
const int ModModel::amount_min = -99;

ModView::ModView(QWidget * parent) : QWidget(parent){
	QLabel * lab;
	mLayout = new QGridLayout(this);

	mAmountSlider = new SliderSpinBox(this);
	mAmountSlider->setRange(ModModel::amount_min, ModModel::amount_max);

	mSrcSelect = new QComboBox(this);
	mDestSelect = new QComboBox(this);

	for(unsigned int i = 0; i < NUM_MODULATION_SOURCES; i++)
		mSrcSelect->addItem(QString(modulation_sources[i]));
	for(unsigned int i = 0; i < NUM_MODULATION_DESTINATIONS; i++)
		mDestSelect->addItem(QString(modulation_destinations[i]));

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
}

std::vector<QLabel *> * ModView::labels(){
	return &mLabels;
}

void ModView::show_labels(bool show){
	for(std::vector<QLabel *>::iterator it = mLabels.begin(); it != mLabels.end(); it++)
		(*it)->setVisible(show);
}


