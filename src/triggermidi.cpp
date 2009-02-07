#include "triggermidi.hpp"
#include "common.hpp"
#include "sliderspinbox.hpp"
#include <QComboBox>
#include <QToolButton>
#include <QGridLayout>
#include <QLabel>

const unsigned int TriggerMIDIModel::trigger_select_max = 13;
const int TriggerMIDIModel::midi_transpose_max = 36;
const int TriggerMIDIModel::midi_transpose_min = -36;
const unsigned int TriggerMIDIModel::midi_key_mode_max = 5;
const unsigned int TriggerMIDIModel::midi_pitch_bend_range_max = 12;
const char * TriggerMIDIModel::trigger_select_names[] = {
	"Envelopes triggered by sequencer or MIDI note",
	"Envelopes triggered by sequencer only", 
	"Envelopes triggered by MIDI notes only",
	"Envelopes triggered by MIDI notes, sequencer reset on Note On",
	"Combo 1: Envelopes trigered by sequencer when MIDI note down",
	"Combo 2: Envelopes trig by seq when MIDI note down, Note On resets seq",
	"External Input gates the envelopes",
	"External Input gates the envelopes and resets the sequencer",
	"External Input gates the sequencer",
	"External Input gates the sequencer and resets the sequencer",
	"Midi note plays sequence once",
	"Midi note plays sequence once, resetting on multiple notes",
	"External Trigger: the sequence plays once on an external signal",
	"The sequence plays once when a key is hit"
};
const char * TriggerMIDIModel::midi_key_mode_names[] = {
	"Low note priority",
	"Low note priority, re-trigger envelopes",
	"High note priority",
	"High note priority, re-trigger envelopes",
	"Last note hit priority",
	"Last note hit priority, re-trigger envelopes"
};

TriggerMIDIModel::TriggerMIDIModel(QObject * parent) : Model(parent) {
	mTriggerSelect = 0;
	mMIDITranspose = 0;
	mMIDIKeysOn = false;
	mMIDIKeyMode = 0;
	mMIDIPitchBendRange = 0;
}

void TriggerMIDIModel::set_trigger_select(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mTriggerSelect, trigger_select_max)){
		mTriggerSelect = val;
		emit(trigger_select_changed(mTriggerSelect));
	}
}

void TriggerMIDIModel::set_midi_transpose(int val){
	if(in_range_and_new<int>(val, mMIDITranspose, midi_transpose_max, midi_transpose_min)){
		mMIDITranspose = val;
		emit(midi_transpose_changed(mMIDITranspose));
	}
}

void TriggerMIDIModel::set_midi_keys_on(bool val){
	if(mMIDIKeysOn != val){
		mMIDIKeysOn = val;
		emit(midi_keys_on_changed(mMIDIKeysOn));
	}
}

void TriggerMIDIModel::set_midi_key_mode(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mMIDIKeyMode, midi_key_mode_max)){
		mMIDIKeyMode = val;
		emit(midi_key_mode_changed(mMIDIKeyMode));
	}
}

void TriggerMIDIModel::set_midi_pitch_bend_range(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mMIDIPitchBendRange, midi_pitch_bend_range_max)){
		mMIDIPitchBendRange = val;
		emit(midi_pitch_bend_range_changed(mMIDIPitchBendRange));
	}
}

//************ view

TriggerMIDIView::TriggerMIDIView(QWidget * parent) : QWidget(parent){
	//allocate
	mLayout = new QGridLayout(this);
	mTriggerSelect = new QComboBox(this);
	mMIDITranspose = new SliderSpinBox(this);
	mMIDIKeysOn = new QToolButton(this);
	mMIDIKeyMode = new QComboBox(this);
	mMIDIPitchBendRange = new SliderSpinBox(this);

	//set up
	for(unsigned int i = 0; i <= TriggerMIDIModel::trigger_select_max; i++)
		mTriggerSelect->addItem(TriggerMIDIModel::trigger_select_names[i]);
	mMIDITranspose->setRange(TriggerMIDIModel::midi_transpose_min, 
			TriggerMIDIModel::midi_transpose_max);
	mMIDIKeysOn->setCheckable(true);
	mMIDIKeysOn->setText("midi keys on");
	for(unsigned int i = 0; i <= TriggerMIDIModel::midi_key_mode_max; i++)
		mMIDIKeyMode->addItem(TriggerMIDIModel::midi_key_mode_names[i]);
	mMIDIPitchBendRange->setRange(0, TriggerMIDIModel::midi_pitch_bend_range_max);

	//layout
	unsigned int row = 0;
	mLayout->addWidget(new QLabel("trigger select", this), ++row, 0, 1, 1, Qt::AlignRight);
	mLayout->addWidget(mTriggerSelect, row, 1);
	mLayout->addWidget(mMIDIKeysOn, ++row, 1);
	mLayout->addWidget(new QLabel("midi transpose", this), ++row, 0, 1, 1, Qt::AlignRight);
	mLayout->addWidget(mMIDITranspose, row, 1);
	mLayout->addWidget(new QLabel("midi key mode", this), ++row, 0, 1, 1, Qt::AlignRight);
	mLayout->addWidget(mMIDIKeyMode, row, 1);
	mLayout->addWidget(new QLabel("midi bend range", this), ++row, 0, 1, 1, Qt::AlignRight);
	mLayout->addWidget(mMIDIPitchBendRange, row, 1);
	mLayout->setRowStretch(row + 1, 10);
	mLayout->setColumnStretch(4, 10);

	setLayout(mLayout);

	//connect signals out
	QObject::connect(mTriggerSelect,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(trigger_select_changed(int)));
	QObject::connect(mMIDITranspose,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(midi_transpose_changed(int)));
	QObject::connect(mMIDIKeysOn,
			SIGNAL(toggled(bool)),
			this,
			SIGNAL(midi_keys_on_changed(bool)));
	QObject::connect(mMIDIKeyMode,
			SIGNAL(currentIndexChanged(int)),
			this,
			SIGNAL(midi_key_mode_changed(int)));
	QObject::connect(mMIDIPitchBendRange,
			SIGNAL(valueChanged(int)),
			this,
			SIGNAL(midi_pitch_bend_range_changed(int)));
}

void TriggerMIDIView::connect_to_model(TriggerMIDIModel * model){
	QObject::connect(
			this,
			SIGNAL(trigger_select_changed(int)),
			model,
			SLOT(set_trigger_select(int)));
	QObject::connect(
			model,
			SIGNAL(trigger_select_changed(int)),
			this,
			SLOT(set_trigger_select(int)));

	QObject::connect(
			this,
			SIGNAL(midi_transpose_changed(int)),
			model,
			SLOT(set_midi_transpose(int)));
	QObject::connect(
			model,
			SIGNAL(midi_transpose_changed(int)),
			this,
			SLOT(set_midi_transpose(int)));

	QObject::connect(
			this,
			SIGNAL(midi_keys_on_changed(bool)),
			model,
			SLOT(set_midi_keys_on(bool)));
	QObject::connect(
			model,
			SIGNAL(midi_keys_on_changed(bool)),
			this,
			SLOT(set_midi_keys_on(bool)));

	QObject::connect(
			this,
			SIGNAL(midi_key_mode_changed(int)),
			model,
			SLOT(set_midi_key_mode(int)));
	QObject::connect(
			model,
			SIGNAL(midi_key_mode_changed(int)),
			this,
			SLOT(set_midi_key_mode(int)));

	QObject::connect(
			this,
			SIGNAL(midi_pitch_bend_range_changed(int)),
			model,
			SLOT(set_midi_pitch_bend_range(int)));
	QObject::connect(
			model,
			SIGNAL(midi_pitch_bend_range_changed(int)),
			this,
			SLOT(set_midi_pitch_bend_range(int)));
}


void TriggerMIDIView::set_trigger_select(int val){
	mTriggerSelect->setCurrentIndex(val);
}

void TriggerMIDIView::set_midi_transpose(int val){
	mMIDITranspose->setValue(val);
}

void TriggerMIDIView::set_midi_keys_on(bool val){
	mMIDIKeysOn->setChecked(val);
}

void TriggerMIDIView::set_midi_key_mode(int val){
	mMIDIKeyMode->setCurrentIndex(val);
}

void TriggerMIDIView::set_midi_pitch_bend_range(int val){
	mMIDIPitchBendRange->setValue(val);
}

