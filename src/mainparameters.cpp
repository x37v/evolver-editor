#include "mainparameters.hpp"
#include "common.hpp"
#include "sliderspinbox.hpp"
#include <QGridLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QToolButton>
#include <QString>
#include <QLabel>
#include <QHBoxLayout>

const unsigned int MainModel::program_number_max = 128;
const unsigned int MainModel::program_number_min = 1;
const unsigned int MainModel::bank_number_max = 4;
const unsigned int MainModel::bank_number_min = 1;
const unsigned int MainModel::master_volume_max = 100;
const int MainModel::master_transpose_max = 36;
const int MainModel::master_transpose_min = -36;
const unsigned int MainModel::bpm_max = 250;
const unsigned int MainModel::bpm_min = 30;
const unsigned int MainModel::clock_divide_max = 12;
const unsigned int MainModel::midi_clock_select_max = 6;
const unsigned int MainModel::poly_chain_select_max = 2;
const unsigned int MainModel::input_gain_max = 8;
const int MainModel::master_fine_tune_max = 50;
const int MainModel::master_fine_tune_min = -50;
const unsigned int MainModel::midi_receive_max = 3;
const unsigned int MainModel::midi_transmit_max = 3;
const unsigned int MainModel::midi_channel_max = 16;

const char * MainModel::clock_divide_names[] = {
	"Half Note",
	"Quarter Note",
	"Eighth Note",
	"Eighth Note half swing",
	"Eighth Note full swing",
	"Eighth Note triplets",
	"Sixteenth Note",
	"Sixteenth Note half swing",
	"Sixteenth Note full swing",
	"Sixteenth Note triplets",
	"Thirty-second Notes",
	"Thirty-second Notes triplets",
	"Sixty-Fourth Notes triplets"
};
const char * MainModel::midi_clock_select_names[] = {
	"Use Internal clock, don't send MIDI clock",
	"Use Internal clock, send MIDI clock",
	"Use MIDI clock In",
	"Use MIDI clock In, and retransmit MIDI clock out",
	"No change",
	"No change",
	"Use MIDI clock In, but ignore MIDI Start/Stop"
};
const char * MainModel::poly_chain_select_names[] = {
	"Normal MIDI operation",
	"All MIDI control data is echoed",
	"Only MIDI note data is echoed"
};
const char * MainModel::input_gain_names[] = {
	"No gain",
	"+3 db",
	"+6 db",
	"+9 db",
	"+12 db",
	"+15 db",
	"+18 db",
	"+21 db",
	"+24 db"
};
const char * MainModel::midi_receive_names[] = {
	"Off: no MIDI is received",
	"All MIDI received",
	"Only MIDI program changes received (+ note/controller data)",
	"Only parameters received (+ note/controller data)"
};
const char * MainModel::midi_transmit_names[] = {
	"Off: no MIDI is transmitted",
	"All MIDI is transmitted",
	"Only Program Changes transmitted",
	"Only parameters are transmitted"
};

MainModel::MainModel(QObject * parent) : QObject(parent){
	mProgramNumber = 1;
	mBankNumber = 1;
	mMasterVolume = 0;
	mBPM = 30;
	mClockDiv = 0;
	mUseProgramTempo = false;
	mMIDIClockSelect  = 0;
	mLockSequence = false;
	mPolyChainSelect = 0;
	mInputGain = 0;
	mMasterFineTune = 0;
	mMIDIReceive = 0;
	mMIDITransmit = 0;
	mMIDIChannel = 0;
}

void MainModel::set_program_number(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mProgramNumber, program_number_max, program_number_min)){
		mProgramNumber = val;
		emit(program_number_changed(mProgramNumber));
	}
}

void MainModel::set_bank_number(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mBankNumber, bank_number_max, bank_number_min)){
		mBankNumber = val;
		emit(bank_number_changed(mBankNumber));
	}
}

void MainModel::set_master_volume(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mMasterVolume, master_volume_max)){
		mMasterVolume = val;
		emit(master_volume_changed(mMasterVolume));
	}
}

void MainModel::set_master_transpose(int val){
	if(in_range_and_new<int>(val, mMasterTranspose, master_transpose_max, master_transpose_min)){
		mMasterTranspose = val;
		emit(master_transpose_changed(mMasterTranspose));
	}
}

void MainModel::set_bpm(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mBPM, bpm_max, bpm_min)){
		mBPM = val;
		emit(bpm_changed(mBPM));
	}
}

void MainModel::set_clock_divide(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mClockDiv, clock_divide_max)){
		mClockDiv = val;
		emit(clock_divide_changed(mClockDiv));
	}
}

void MainModel::set_use_program_tempo(bool val){
	if(val != mUseProgramTempo){
		mUseProgramTempo = val;
		emit(use_program_tempo_changed(mUseProgramTempo));
	}
}

void MainModel::set_midi_clock_select(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mMIDIClockSelect, midi_clock_select_max)){
		mMIDIClockSelect = val;
		emit(midi_clock_select_changed(mMIDIClockSelect));
	}
}

void MainModel::set_lock_sequence(bool val){
	if(val != mLockSequence){
		mLockSequence = val;
		emit(lock_sequence_changed(mLockSequence));
	}
}

void MainModel::set_poly_chain_select(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mPolyChainSelect, poly_chain_select_max)){
		mPolyChainSelect = val;
		emit(poly_chain_select_changed(mPolyChainSelect));
	}
}

void MainModel::set_input_gain(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mInputGain, input_gain_max)){
		mInputGain = val;
		emit(input_gain_changed(mInputGain));
	}
}

void MainModel::set_master_fine_tune(int val){
	if(in_range_and_new<int>(val, mMasterFineTune, master_fine_tune_max, master_fine_tune_min)){
		mMasterFineTune = val;
		emit(master_fine_tune_changed(mMasterFineTune));
	}
}

void MainModel::set_midi_receive(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mMIDIReceive, midi_receive_max)){
		mMIDIReceive = val;
		emit(midi_receive_changed(mMIDIReceive));
	}
}

void MainModel::set_midi_transmit(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mMIDITransmit, midi_transmit_max)){
		mMIDITransmit = val;
		emit(midi_transmit_changed(mMIDITransmit));
	}
}

void MainModel::set_midi_channel(int val){
	if(in_range_and_new<unsigned int>((unsigned int)val, mMIDIChannel, midi_channel_max)){
		mMIDIChannel = val;
		emit(midi_channel_changed(mMIDIChannel));
	}
}

MainView::MainView(QWidget * parent) : QWidget(parent){
	//allocate
	mLayout = new QGridLayout(this);
	mProgramNumber = new QSpinBox(this);
	mBankNumber = new QSpinBox(this);
	mMasterVolume = new SliderSpinBox(this);
	mMasterTranspose = new SliderSpinBox(this);
	mBPM = new SliderSpinBox(this);
	mClockDiv = new QComboBox(this);
	mUseProgramTempo = new QToolButton(this);
	mMIDIClockSelect = new QComboBox(this);
	mLockSequence = new QToolButton(this);
	mPolyChainSelect = new QComboBox(this);
	mInputGain = new QComboBox(this);
	mMasterFineTune = new SliderSpinBox(this);
	mMIDIReceive = new QComboBox(this);
	mMIDITransmit = new QComboBox(this);
	mMIDIChannel = new QComboBox(this);

	//set up
	mProgramNumber->setRange(MainModel::program_number_min, MainModel::program_number_max);
	mBankNumber->setRange(MainModel::bank_number_min, MainModel::bank_number_max);
	mMasterVolume->setRange(0, MainModel::master_volume_max);
	mMasterTranspose->setRange(MainModel::master_transpose_min, MainModel::master_volume_max);
	mBPM->setRange(MainModel::bpm_min, MainModel::bpm_max);
	for(unsigned int i = 0; i <= MainModel::clock_divide_max; i++)
		mClockDiv->addItem(MainModel::clock_divide_names[i]);
	mUseProgramTempo->setCheckable(true);
	mUseProgramTempo->setText("use program tempo");
	for(unsigned int i = 0; i <= MainModel::midi_clock_select_max; i++)
		mMIDIClockSelect->addItem(MainModel::midi_clock_select_names[i]);
	mLockSequence->setCheckable(true);
	mLockSequence->setText("lock sequence");
	for(unsigned int i = 0; i <= MainModel::poly_chain_select_max; i++)
		mPolyChainSelect->addItem(MainModel::poly_chain_select_names[i]);
	for(unsigned int i = 0; i <= MainModel::input_gain_max; i++)
		mInputGain->addItem(MainModel::input_gain_names[i]);
	mMasterFineTune->setRange(MainModel::master_fine_tune_min, MainModel::master_fine_tune_max);
	for(unsigned int i = 0; i <= MainModel::midi_receive_max; i++)
		mMIDIReceive->addItem(MainModel::midi_receive_names[i]);
	for(unsigned int i = 0; i <= MainModel::midi_transmit_max; i++)
		mMIDITransmit->addItem(MainModel::midi_transmit_names[i]);
	mMIDIChannel->addItem("omni");
	for(unsigned int i = 1; i <= MainModel::midi_channel_max; i++)
		mMIDIChannel->addItem(QString("%1").arg(i));

	//layout
	unsigned int row = 0;
	QHBoxLayout * progLayout = new QHBoxLayout;

	progLayout->addWidget(mProgramNumber);
	progLayout->addSpacing(2);
	progLayout->addWidget(new QLabel("bank number"));
	progLayout->addWidget(mBankNumber);
	progLayout->addStretch(10);
	progLayout->setContentsMargins(0,0,0,0);

	mLayout->addWidget(new QLabel("program number", this), ++row, 0, 1, 1, Qt::AlignRight);
	mLayout->addLayout(progLayout, row, 1);

	mLayout->addWidget(new QLabel("master volume", this), ++row, 0, 1, 1, Qt::AlignRight);
	mLayout->addWidget(mMasterVolume, row, 1);
	mLayout->addWidget(new QLabel("master transpose", this), ++row, 0, 1, 1, Qt::AlignRight);
	mLayout->addWidget(mMasterTranspose, row, 1);
	mLayout->addWidget(new QLabel("master tune", this), ++row, 0, 1, 1, Qt::AlignRight);
	mLayout->addWidget(mMasterFineTune, row, 1);
	mLayout->addWidget(new QLabel("input gain", this), ++row, 0, 1, 1, Qt::AlignRight);
	mLayout->addWidget(mInputGain, row, 1);

	mLayout->addWidget(new QLabel("clock divide", this), ++row, 0, 1, 1, Qt::AlignRight);
	mLayout->addWidget(mClockDiv, row, 1);
	mLayout->addWidget(new QLabel("bpm", this), ++row, 0, 1, 1, Qt::AlignRight);
	mLayout->addWidget(mBPM, row, 1);

	QHBoxLayout * buttonLayout = new QHBoxLayout;
	buttonLayout->addWidget(mUseProgramTempo);
	buttonLayout->addWidget(mLockSequence);
	buttonLayout->addStretch(10);
	buttonLayout->setContentsMargins(0,0,0,0);
	mLayout->addLayout(buttonLayout, ++row, 1);
	//mLayout->addWidget(mUseProgramTempo, ++row, 1, 1, 1, Qt::AlignLeft);
	//mLayout->addWidget(mLockSequence, ++row, 1, 1, 1, Qt::AlignLeft);

	mLayout->addWidget(new QLabel("poly chain", this), ++row, 0, 1, 1, Qt::AlignRight);
	mLayout->addWidget(mPolyChainSelect, row, 1);

	mLayout->addWidget(new QLabel("midi clock", this), ++row, 0, 1, 1, Qt::AlignRight);
	mLayout->addWidget(mMIDIClockSelect, row, 1);
	mLayout->addWidget(new QLabel("midi receive", this), ++row, 0, 1, 1, Qt::AlignRight);
	mLayout->addWidget(mMIDIReceive, row, 1);
	mLayout->addWidget(new QLabel("midi transmit", this), ++row, 0, 1, 1, Qt::AlignRight);
	mLayout->addWidget(mMIDITransmit, row, 1);
	mLayout->addWidget(new QLabel("midi channel", this), ++row, 0, 1, 1, Qt::AlignRight);
	mLayout->addWidget(mMIDIChannel, row, 1);

	mLayout->setRowStretch(row + 1, 10);
	mLayout->setColumnStretch(4, 10);

	setLayout(mLayout);
}

void MainView::set_program_number(int val){
	mProgramNumber->setValue(val);
}

void MainView::set_bank_number(int val){
	mBankNumber->setValue(val);
}

void MainView::set_master_volume(int val){
	mMasterVolume->setValue(val);
}

void MainView::set_master_transpose(int val){
	mMasterTranspose->setValue(val);
}

void MainView::set_bpm(int val){
	mBPM->setValue(val);
}

void MainView::set_clock_divide(int val){
	mClockDiv->setCurrentIndex(val);
}

void MainView::set_use_program_tempo(bool val){
	mUseProgramTempo->setChecked(val);
}

void MainView::set_midi_clock_select(int val){
	mMIDIClockSelect->setCurrentIndex(val);
}

void MainView::set_lock_sequence(bool val){
	mLockSequence->setChecked(val);
}

void MainView::set_poly_chain_select(int val){
	mPolyChainSelect->setCurrentIndex(val);
}

void MainView::set_input_gain(int val){
	mInputGain->setCurrentIndex(val);
}

void MainView::set_master_fine_tune(int val){
	mMasterFineTune->setValue(val);
}

void MainView::set_midi_receive(int val){
	mMIDIReceive->setCurrentIndex(val);
}

void MainView::set_midi_transmit(int val){
	mMIDITransmit->setCurrentIndex(val);
}

void MainView::set_midi_channel(int val){
	mMIDIChannel->setCurrentIndex(val);
}

void MainView::connect_to_model(MainModel * model){
	QObject::connect(
			this,
			SIGNAL(program_number_changed(int)),
			model,
			SLOT(set_program_number(int)));
	QObject::connect(
			model,
			SIGNAL(program_number_changed(int)),
			this,
			SLOT(set_program_number(int)));

	QObject::connect(
			this,
			SIGNAL(bank_number_changed(int)),
			model,
			SLOT(set_bank_number(int)));
	QObject::connect(
			model,
			SIGNAL(bank_number_changed(int)),
			this,
			SLOT(set_bank_number(int)));

	QObject::connect(
			this,
			SIGNAL(master_volume_changed(int)),
			model,
			SLOT(set_master_volume(int)));
	QObject::connect(
			model,
			SIGNAL(master_volume_changed(int)),
			this,
			SLOT(set_master_volume(int)));

	QObject::connect(
			this,
			SIGNAL(master_transpose_changed(int)),
			model,
			SLOT(set_master_transpose(int)));
	QObject::connect(
			model,
			SIGNAL(master_transpose_changed(int)),
			this,
			SLOT(set_master_transpose(int)));

	QObject::connect(
			this,
			SIGNAL(bpm_changed(int)),
			model,
			SLOT(set_bpm(int)));
	QObject::connect(
			model,
			SIGNAL(bpm_changed(int)),
			this,
			SLOT(set_bpm(int)));

	QObject::connect(
			this,
			SIGNAL(clock_divide_changed(int)),
			model,
			SLOT(set_clock_divide(int)));
	QObject::connect(
			model,
			SIGNAL(clock_divide_changed(int)),
			this,
			SLOT(set_clock_divide(int)));

	QObject::connect(
			this,
			SIGNAL(use_program_tempo_changed(bool)),
			model,
			SLOT(set_use_program_tempo(bool)));
	QObject::connect(
			model,
			SIGNAL(use_program_tempo_changed(bool)),
			this,
			SLOT(set_use_program_tempo(bool)));

	QObject::connect(
			this,
			SIGNAL(midi_clock_select_changed(int)),
			model,
			SLOT(set_midi_clock_select(int)));
	QObject::connect(
			model,
			SIGNAL(midi_clock_select_changed(int)),
			this,
			SLOT(set_midi_clock_select(int)));

	QObject::connect(
			this,
			SIGNAL(lock_sequence_changed(bool)),
			model,
			SLOT(set_lock_sequence(bool)));
	QObject::connect(
			model,
			SIGNAL(lock_sequence_changed(bool)),
			this,
			SLOT(set_lock_sequence(bool)));

	QObject::connect(
			this,
			SIGNAL(poly_chain_select_changed(int)),
			model,
			SLOT(set_poly_chain_select(int)));
	QObject::connect(
			model,
			SIGNAL(poly_chain_select_changed(int)),
			this,
			SLOT(set_poly_chain_select(int)));

	QObject::connect(
			this,
			SIGNAL(input_gain_changed(int)),
			model,
			SLOT(set_input_gain(int)));
	QObject::connect(
			model,
			SIGNAL(input_gain_changed(int)),
			this,
			SLOT(set_input_gain(int)));

	QObject::connect(
			this,
			SIGNAL(master_fine_tune_changed(int)),
			model,
			SLOT(set_master_fine_tune(int)));
	QObject::connect(
			model,
			SIGNAL(master_fine_tune_changed(int)),
			this,
			SLOT(set_master_fine_tune(int)));

	QObject::connect(
			this,
			SIGNAL(midi_receive_changed(int)),
			model,
			SLOT(set_midi_receive(int)));
	QObject::connect(
			model,
			SIGNAL(midi_receive_changed(int)),
			this,
			SLOT(set_midi_receive(int)));

	QObject::connect(
			this,
			SIGNAL(midi_transmit_changed(int)),
			model,
			SLOT(set_midi_transmit(int)));
	QObject::connect(
			model,
			SIGNAL(midi_transmit_changed(int)),
			this,
			SLOT(set_midi_transmit(int)));

	QObject::connect(
			this,
			SIGNAL(midi_channel_changed(int)),
			model,
			SLOT(set_midi_channel(int)));
	QObject::connect(
			model,
			SIGNAL(midi_channel_changed(int)),
			this,
			SLOT(set_midi_channel(int)));

}

