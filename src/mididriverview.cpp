#include "mididriverview.hpp"
#include "mididriver.hpp"
#include <QComboBox>
#include <QGridLayout>
#include <QLabel>

MidiDriverView::MidiDriverView(QWidget * parent) : QWidget(parent){
	mInputSelect = new QComboBox(this);
	mOutputSelect = new QComboBox(this);
	mLayout = new QGridLayout(this);

	mLayout->addWidget(new QLabel("input device: ", this), 1, 0, 1, 1, Qt::AlignRight);
	mLayout->addWidget(mInputSelect, 1, 1);
	mLayout->addWidget(new QLabel("output device: ", this), 2, 0, 1, 1, Qt::AlignRight);
	mLayout->addWidget(mOutputSelect, 2, 1);

	mInputSelect->addItem("no selection", -1);
	mOutputSelect->addItem("no selection", -1);

	mLayout->setRowStretch(4, 10);
	mLayout->setColumnStretch(4, 10);

	setLayout(mLayout);

	//connect internal signals/slots
	QObject::connect(mInputSelect, SIGNAL(currentIndexChanged(int)),
			this, SLOT(set_current_input_index(int)));
	QObject::connect(mOutputSelect, SIGNAL(currentIndexChanged(int)),
			this, SLOT(set_current_output_index(int)));
}

void MidiDriverView::update_inputs(const std::map<unsigned int, QString> * inputs){
	mInputSelect->clear();
	mInputSelect->addItem("no selection", -1);
	for(std::map<unsigned int, QString>::const_iterator it = inputs->begin(); it != inputs->end(); it++)
		mInputSelect->addItem(it->second, (int)it->first);
}

void MidiDriverView::update_outputs(const std::map<unsigned int, QString> * outputs){
	mOutputSelect->clear();
	mOutputSelect->addItem("no selection", -1);
	for(std::map<unsigned int, QString>::const_iterator it = outputs->begin(); it != outputs->end(); it++)
		mOutputSelect->addItem(it->second, (int)it->first);
}

void MidiDriverView::set_input_index(int index){
	int my_index = mInputSelect->findData(index);
	if(my_index >= 0)
		mInputSelect->setCurrentIndex(my_index);
}

void MidiDriverView::set_output_index(int index){
	int my_index = mOutputSelect->findData(index);
	if(my_index >= 0)
		mOutputSelect->setCurrentIndex(my_index);
}


void MidiDriverView::set_current_input_index(int index){
	emit(input_index_changed(mInputSelect->itemData(index).toInt()));
}

void MidiDriverView::set_current_output_index(int index){
	emit(output_index_changed(mOutputSelect->itemData(index).toInt()));
}

