#include "mainparameters.hpp"
#include "common.hpp"

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
	"Use Internal clock, don’t send MIDI clock",
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
	"Only MIDI program changes received (along with note/controller data)",
	"Only parameters received (along with note/controller data)"
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

