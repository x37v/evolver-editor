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

MiscModulationModel::MiscModulationModel(QObject * parent) : QObject(parent) {
	mOscSlop = 0;
	for(unsigned int i = 0; i < num_modulation_sources; i++){
		mAmounts.push_back(0);
		mDestinations.push_back(0);
	}
}

#include "sliderspinbox.hpp"
#include "modulation_destinations.hpp"
#include <QGridLayout>
#include <QLabel>

MiscModulationView::MiscModulationView(QWidget * parent) : QWidget(parent){
	QLabel * lab;
	//allocate
	mLayout = new QGridLayout(this);

	mOscSlop = new SliderSpinBox(this);

	for(unsigned int i = 0; i < MiscModulationModel::num_modulation_sources; i++){
		SliderSpinBox * a = new SliderSpinBox(this);
		mAmounts.push_back(a);
		mDestinations.push_back(new ModDestComboBox(this));
		a->setRange(MiscModulationModel::amount_min, MiscModulationModel::amount_max);
	}

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
}
