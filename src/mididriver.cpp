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

#include "mididriver.hpp"
#include "portmidi.h"
#include "applicationmodel.hpp"
#include "oscmodel.hpp"
#include "filter.hpp"
#include "vca.hpp"
#include "miscaudio.hpp"
#include "env3.hpp"
#include "modulators.hpp"
#include "miscmodulation.hpp"
#include "delay.hpp"
#include "feedback.hpp"
#include "lfo.hpp"
#include "sequencer.hpp"
#include "mainparameters.hpp"
#include <string.h>
#include <QTimer>

const uint8_t MidiDriver::evolver_sysex_header[] = { 0x01, 0x20, 0x01 };

#define is_real_time_msg(msg) ((0xF0 & Pm_MessageStatus(msg)) == 0xF8)
#define MIDI_SYSEX_START 0xF0
#define MIDI_SYSEX_END 0xF7

#define invoke_method(ob, meth, ...) (QMetaObject::invokeMethod(ob, #meth, Qt::QueuedConnection, __VA_ARGS__))

MidiDriver::MidiDriver(ApplicationModel * model, QObject * parent) : QThread(parent){
	mMidiOut = NULL;
	mMidiIn = NULL;
	mInputIndex = -1;
	mOutputIndex = -1;
	mReading = false;
	mModel = model;
	mTimer = new QTimer;
	connect(mTimer, SIGNAL(timeout()), this, SLOT(poll()));

	update_device_list();

	setTerminationEnabled();
}

const std::map<unsigned int, QString> * MidiDriver::input_map(){
	return &mInputMap;
}

const std::map<unsigned int, QString> * MidiDriver::output_map(){
	return &mOutputMap;
}

void MidiDriver::run(){
	if(mMidiIn){
		//ms
		mTimer->start(2);
		exec();
	}
}

void MidiDriver::request_update_state(){
	request_edit_buffer();
	request_main_params();
}

void MidiDriver::request_main_params(){
	if(mMidiOut){
		uint8_t msg[8];
		msg[0] = (uint8_t)MIDI_SYSEX_START;
		memcpy(msg + 1, evolver_sysex_header, 3);
		msg[4] = main_param_dump_request;
		msg[5] = (uint8_t)MIDI_SYSEX_END;
		Pm_WriteSysEx(mMidiOut, 0, msg);
	}
}

void MidiDriver::request_edit_buffer(){
	if(mMidiOut){
		uint8_t msg[8];
		msg[0] = (uint8_t)MIDI_SYSEX_START;
		memcpy(msg + 1, evolver_sysex_header, 3);
		msg[4] = edit_dump_request;
		msg[5] = (uint8_t)MIDI_SYSEX_END;
		Pm_WriteSysEx(mMidiOut, 0, msg);
	}
}

void MidiDriver::request_program(uint8_t program, uint8_t bank){
	if(mMidiOut){
		uint8_t msg[9];
		msg[0] = (uint8_t)MIDI_SYSEX_START;
		memcpy(msg + 1, evolver_sysex_header, 3);
		msg[4] = prog_dump_request;
		msg[5] = 0x03 & bank;
		msg[6] = 0x7F & program;
		msg[7] = (uint8_t)MIDI_SYSEX_END;
		Pm_WriteSysEx(mMidiOut, 0, msg);
	}
}

void MidiDriver::request_waveform_dump(int waveform){
	if(waveform < 0 || waveform > 127)
		return;
	if(mMidiOut){
		uint8_t msg[8];
		msg[0] = (uint8_t)MIDI_SYSEX_START;
		memcpy(msg + 1, evolver_sysex_header, 3);
		msg[4] = wave_dump_request;
		msg[5] = (uint8_t)waveform & 0x7F;
		msg[6] = (uint8_t)MIDI_SYSEX_END;
		Pm_WriteSysEx(mMidiOut, 0, msg);
	}
}

void MidiDriver::poll(){
	PmEvent msg;
	PmError count;
	count = Pm_Read(mMidiIn, &msg, 1);

	while(count > 0){
		/* ignore real-time messages */
		if (is_real_time_msg(Pm_MessageStatus(msg.message))) 
			return;
		/* write 4 bytes of data until you reach an eox */
		for (unsigned int shift = 0; shift < 32; shift += 8) {
			uint8_t data = (msg.message >> shift) & 0xFF;
			if(data == MIDI_SYSEX_START){
				mReadingCount = 0;
				mReading = true;
			} else if (data == MIDI_SYSEX_END){
				mReading = false;
			} else if (mReading){
				if(mReadingCount < 3){
					if(data != evolver_sysex_header[mReadingCount])
						mReading = false;
				} else if(mReadingCount == 3) {
					//command
					switch(data){
						case prog_param:
							mCurrentCommand = prog_param;
							break;
						case prog_dump:
							mCurrentCommand = prog_dump;
							mInputBuffer.clear();
							break;
						case edit_dump:
							mCurrentCommand = edit_dump;
							mInputBuffer.clear();
							break;
						case main_param_dump:
							mCurrentCommand = main_param_dump;
							mInputBuffer.clear();
							break;
						case seq_param:
							mCurrentCommand = seq_param;
							break;
						case main_param:
							mCurrentCommand = main_param;
							break;
						default:
							//XXX haven't implemented all the commands yet
							mReading = false;
					};
				} else {
					//after the header, this is our b.s.
					unsigned int index = mReadingCount - 4;
					switch(mCurrentCommand){
						case prog_dump:
							if(index == 0){
								//bank number
								//XXX we requested this program, so we don't have to actually set it
								//invoke_method(mModel->main(), set_bank_number, Q_ARG(int, data + 1));
								break;
							} else if (index == 1){
								//program number
								//XXX we requested this program, so we don't have to actually set it
								//invoke_method(mModel->main(), set_program_number, Q_ARG(int, data + 1));
								break;
							}
							//update the index for the prog_param, let it fall through
							index -= 2;
						case edit_dump:
							mInputBuffer.push_back(data);
							//there are 220 bytes of data in a dump..
							if(index >= 219){
								mReading = false;
								//unpack && update our parameters
								std::vector<uint8_t> unpacked;
								unpack_data(mInputBuffer, unpacked);
								for(unsigned int i = 0; i < 127; i++)
									update_program_param(i, unpacked[i]);
								//clear all the resets
								QMetaObject::invokeMethod(mModel->sequencer(), "clear_sequence_resets", Qt::QueuedConnection);
								//we go backwards so that our lengths are correct
								for(int i = 63; i >= 0; i--)
									update_sequence_param(i, unpacked[i + 128]);
							}
							break;
						case main_param_dump:
							//main parmeters are not packed, they come "index, "value" 
							//so every other can be sent to the model
							if(index % 2 == 0){
								mInputParamValue = data;
							} else 
								update_main_param(index >> 1, (data << 4) | mInputParamValue);
							break;
						case prog_param:
							switch(index){
								case 0:
									mInputParamNumber = data;
									break;
								case 1:
									mInputParamValue = data;
									break;
								case 2:
									mInputParamValue |= (data << 4);
									//update the shit
									update_program_param(mInputParamNumber, mInputParamValue);
									break;
								default:
									break;
							};
							break;
						case seq_param:
							switch(index){
								case 0:
									mInputParamNumber = data;
									break;
								case 1:
									mInputParamValue = data;
									break;
								case 2:
									mInputParamValue |= (data << 4);
									//update the shit
									update_sequence_param(mInputParamNumber, mInputParamValue);
									break;
								default:
									break;
							};
						case main_param:
							switch(index){
								case 0:
									mInputParamNumber = data;
									break;
								case 1:
									mInputParamValue = data;
									break;
								case 2:
									mInputParamValue |= (data << 4);
									//update the shit
									update_main_param(mInputParamNumber, mInputParamValue);
									break;
								default:
									break;
							};
							break;
						default: 
							break;
					};
				}
				//increment the count
				mReadingCount++;
			}
		}
		//read some more
		count = Pm_Read(mMidiIn, &msg, 1);
	}
}

void MidiDriver::open_input(QString name) throw(std::runtime_error){
	bool found = false;
	for(std::map<unsigned int, QString>::iterator it = mInputMap.begin();
			it != mInputMap.end(); it++){
		if(it->second == name){
			open_input(it->first);
			found = true;
			break;
		}
	}
	if(!found){
		std::string msg("Cannot find MIDI input with name: ");
		msg.append(name.toStdString());
		throw std::runtime_error(msg);
	}
}

void MidiDriver::open_input(int index){
	if(index != mInputIndex){
		close_input();
		if(index >= 0)
			Pm_OpenInput(&mMidiIn, index, NULL, 512, NULL, NULL);
		mInputIndex = index;
		emit(input_index_changed(mInputIndex));
	}
}

void MidiDriver::open_output(QString name) throw(std::runtime_error){
	bool found = false;
	for(std::map<unsigned int, QString>::iterator it = mOutputMap.begin();
			it != mOutputMap.end(); it++){
		if(it->second == name){
			open_output(it->first);
			found = true;
			break;
		}
	}
	if(!found){
		std::string msg("Cannot find MIDI output with name: ");
		msg.append(name.toStdString());
		throw std::runtime_error(msg);
	}
}

void MidiDriver::open_output(int index){
	if(index != mOutputIndex){
		close_output();
		if(index >= 0)
			Pm_OpenOutput(&mMidiOut, index, NULL, 512, NULL, NULL, 0);
		mOutputIndex = index;
		emit(output_index_changed(mOutputIndex));
	}
}

void MidiDriver::open(int input, int output){
	open_input(input);
	open_output(output);
}

void MidiDriver::close_input(){
	if(mMidiIn)
		Pm_Close(mMidiIn);
	mMidiIn = NULL;
}

void MidiDriver::close_output(){
	if(mMidiOut)
		Pm_Close(mMidiOut);
	mMidiOut = NULL;
}

void MidiDriver::close(){
	close_input();
	close_output();
}


MidiDriver::~MidiDriver(){
	quit();
	mTimer->stop();
	delete mTimer;
	close();
}

void MidiDriver::update_program_param(uint8_t index, uint8_t value){
	switch(index){
		case 0:
			//0 0 - 120 Oscillator 1 Frequency, 0 – 120 in semitones (10 octave range)
			invoke_method(mModel->analog_oscs()->at(0), set_freq, Q_ARG(int, value));
			break;
		case 1:
			//1 0 - 100 Oscillator 1 Fine Tune; 0 = -50 cents, 50 = 0 (centered), 100 = + 50 cents
			invoke_method(mModel->analog_oscs()->at(0), set_tune, Q_ARG(int, (int)value - 50));
			break;
		case 2:
			//2 0 - 102 Oscillator 1 Shape
			//          0    Sawtooth
			//          1    Triangle
			//          2    Sawtooth/triangle mix
			//          3 – 102 Pulse Wave, Pulse width 0 - 99
			if(value < 3)
				invoke_method(mModel->analog_oscs()->at(0), set_shape, Q_ARG(int, value));
			else {
				invoke_method(mModel->analog_oscs()->at(0), set_shape, Q_ARG(int, AnalogOscModel::pulse));
				invoke_method(mModel->analog_oscs()->at(0), set_width, Q_ARG(int, value - 3));
			}
			break;
		case 3:
			//3 0 - 100 Oscillator 1 Level
			invoke_method(mModel->analog_oscs()->at(0), set_level, Q_ARG(int, value));
			break;
		case 4:
			//4 0 - 120 Oscillator 2 Frequency, 0 – 120 in semitones (10 octave range)
			invoke_method(mModel->analog_oscs()->at(1), set_freq, Q_ARG(int, value));
			break;
		case 5:
			//5 0 - 100 Oscillator 2 Fine Tune; 0 = -50 cents, 50 = 0 (centered), 100 = + 50 cents
			invoke_method(mModel->analog_oscs()->at(1), set_tune, Q_ARG(int, (int)value - 50));
			break;
		case 6:
			//6 0 - 102 Oscillator 2 Shape
			//          0    Sawtooth
			//          1    Triangle
			//          2    Sawtooth/triangle mix
			//          3 – 102 Pulse Wave, Pulse width 0 - 99
			if(value < 3)
				invoke_method(mModel->analog_oscs()->at(1), set_shape, Q_ARG(int, value));
			else {
				invoke_method(mModel->analog_oscs()->at(1), set_shape, Q_ARG(int, AnalogOscModel::pulse));
				invoke_method(mModel->analog_oscs()->at(1), set_width, Q_ARG(int, value - 3));
			}
			break;
		case 7:
			//7 0 - 100 Oscillator 2 Level
			invoke_method(mModel->analog_oscs()->at(1), set_level, Q_ARG(int, value));
			break;
		case 8:
			//8     0 - 120 Oscillator 3 Frequency, 0 – 120 in semitones (10 octave range)
			invoke_method(mModel->digital_oscs()->at(0), set_freq, Q_ARG(int, value));
			break;
		case 9:
			//9     0 - 100 Oscillator 3 Fine Tune; 0 = -50 cents, 50 = 0 (centered), 100 = + 50 cents
			invoke_method(mModel->digital_oscs()->at(0), set_tune, Q_ARG(int, (int)value - 50));
			break;
		case 10:
			//10     0 - 127 Oscillator 3 Shape 1 - 128
			invoke_method(mModel->digital_oscs()->at(0), set_shape, Q_ARG(int, value + 1));
			break;
		case 11:
			//11     0 - 100 Oscillator 3 Level
			invoke_method(mModel->digital_oscs()->at(0), set_level, Q_ARG(int, value));
			break;
		case 12:
			//12     0 - 120 Oscillator 4 Frequency, 0 – 120 in semitones (10 octave range)
			invoke_method(mModel->digital_oscs()->at(1), set_freq, Q_ARG(int, value));
			break;
		case 13:
			//13     0 - 100 Oscillator 4 Fine Tune; 0 = -50 cents, 50 = 0 (centered), 100 = + 50 cents
			invoke_method(mModel->digital_oscs()->at(1), set_tune, Q_ARG(int, (int)value - 50));
			break;
		case 14:
			//14 0 - 127 Oscillator 4 Shape 1 - 128
			invoke_method(mModel->digital_oscs()->at(1), set_shape, Q_ARG(int, value + 1));
			break;
		case 15:
			//15 0 - 100 Oscillator 4 Level
			invoke_method(mModel->digital_oscs()->at(1), set_level, Q_ARG(int, value));
			break;
		case 16:
			//16     0 - 164 Filter Frequency, steps in semitones
			invoke_method(mModel->filter(), set_cutoff, Q_ARG(int, value));
			break;
		case 17:
			//17     0 - 198 Filter Envelope Amount; -99 to +99
			invoke_method(mModel->filter(), set_env_amount, Q_ARG(int, (int)value - 99));
			break;
		case 18:
			//18     0 - 110 Filter Envelope Attack
			invoke_method(mModel->filter(), set_env_attack, Q_ARG(int, value));
			break;
		case 19:
			//19     0 - 110 Filter Envelope Decay
			invoke_method(mModel->filter(), set_env_decay, Q_ARG(int, value));
			break;
		case 20:
			//20     0 - 100 Filter Envelope Sustain
			invoke_method(mModel->filter(), set_env_sustain, Q_ARG(int, value));
			break;
		case 21:
			//21     0 - 110 Filter Envelope Release
			invoke_method(mModel->filter(), set_env_release, Q_ARG(int, value));
			break;
		case 22:
			//22     0 - 100 Resonance
			invoke_method(mModel->filter(), set_resonance, Q_ARG(int, value));
			break;
		case 23:
			//23     0 - 100 Filter Keyboard Amount
			invoke_method(mModel->filter(), set_key_amount, Q_ARG(int, value));
			break;
		case 24:
			//24     0 - 100 VCA Level
			invoke_method(mModel->vca(), set_level, Q_ARG(int, value));
			break;
		case 25:
			//25     0 - 100 VCA Envelope Amount
			invoke_method(mModel->vca(), set_env_amount, Q_ARG(int, value));
			break;
		case 26:
			//26     0 - 110 VCA Envelope Attack
			invoke_method(mModel->vca(), set_attack, Q_ARG(int, value));
			break;
		case 27:
			//27     0 - 110 VCA Envelope Decay
			invoke_method(mModel->vca(), set_decay, Q_ARG(int, value));
			break;
		case 28:
			//28     0 - 100 VCA Envelope Sustain
			invoke_method(mModel->vca(), set_sustain, Q_ARG(int, value));
			break;
		case 29:
			//29     0 - 110 VCA Envelope Release
			invoke_method(mModel->vca(), set_release, Q_ARG(int, value));
			break;
		case 30:
			//30     0-6     Output Pan
			//               0   Left channel panned fully left, Right fully to the right
			//               1   Left channel panned mostly left, Right mostly to the right
			//               2   Left channel panned somewhat left, Right somewhat to the right
			//               3   Mono
			//               4   Right channel panned somewhat left, Left somewhat to the right
			//               5   Right channel panned mostly left, Left mostly to the right
			//               6   Right channel panned fully left, Left fully to the right
			invoke_method(mModel->vca(), set_pan, Q_ARG(int, value));
			break;
		case 31:
			//31     0 - 100 Program Volume
			invoke_method(mModel->misc_audio(), set_volume, Q_ARG(int, value));
			break;
		case 32:
			//32     0 - 48  Feedback Frequency – steps in semitones
			invoke_method(mModel->feedback(), set_freq, Q_ARG(int, value));
			break;
		case 33:
			//33     0 - 100 Feedback Amount
			invoke_method(mModel->feedback(), set_level, Q_ARG(int, value));
			break;
		case 34:
			//34     0-1     Grunge; 0 = off, 1 = on
			invoke_method(mModel->feedback(), set_grunge, Q_ARG(bool, (bool)value));
			break;
		case 35:
			//35     0 - 166 Delay 1 Time
			if(value <= 150) {
				invoke_method(mModel->delay(), set_delay_sync, Q_ARG(unsigned int, 0), Q_ARG(int, 0));
				invoke_method(mModel->delay(), set_delay_time, Q_ARG(unsigned int, 0), Q_ARG(int, value));
			} else {
				invoke_method(mModel->delay(), set_delay_sync, Q_ARG(unsigned int, 0), Q_ARG(int, value - 150));
			}
			break;
		case 36:
			//36     0 - 100 Delay 1 Level
			invoke_method(mModel->delay(), set_delay_level, Q_ARG(unsigned int, 0), Q_ARG(int, value));
			break;
		case 37:
			//37     0 - 100 Delay sum feedback to Delay input
			invoke_method(mModel->delay(), set_feedback_level, Q_ARG(int, value));
			break;
		case 38:
			//38     0 - 100 Delay sum feedback to filter input
			invoke_method(mModel->delay(), set_filter_feedback_level, Q_ARG(int, value));
			break;
		case 39:
			//39     0 - 14  Output hack amount
			invoke_method(mModel->misc_audio(), set_output_hack, Q_ARG(int, value));
			break;
		case 40:
			//40     0 - 160 LFO 1 Frequency; 0 – 150 unsynced frequency
			//                  151 Sequence speed divided by 32
			//                  152 Sequence speed divided by 16
			//                  153 Sequence speed divided by 8
			//                  154 Sequence speed divided by 4
			//                  155 Sequence speed divided by 2
			//                  156 One cycle per step
			//                  157 Two cycles per step
			//                  158 Four cycles per step
			//                  159 Eight cycles per step
			//                  160 Sixteen cycles per step
			if(value <= 150){
				invoke_method(mModel->lfos()->at(0), set_sync_type, Q_ARG(int, 0));
				invoke_method(mModel->lfos()->at(0), set_freq, Q_ARG(int, value));
			} else {
				invoke_method(mModel->lfos()->at(0), set_sync_type, Q_ARG(int, value - 150));
			}
			break;
		case 41:
			//41     0-4     LFO 1 Shape
			//                  0   Triangle
			//                  1   Reverse Sawtooth
			//                  2   Sawtooth
			//                  3   Pulse (square)
			//                  4   Random
			invoke_method(mModel->lfos()->at(0), set_shape, Q_ARG(int, value));
			break;
		case 42:
			//42     0 - 200 LFO 1 Amount (over 100 repeats with Key sync on)
			if(value <= 100){
				invoke_method(mModel->lfos()->at(0), set_amount, Q_ARG(int, value));
				invoke_method(mModel->lfos()->at(0), set_key_sync, Q_ARG(bool, false));
			} else {
				invoke_method(mModel->lfos()->at(0), set_key_sync, Q_ARG(bool, true));
				invoke_method(mModel->lfos()->at(0), set_amount, Q_ARG(int, value - 100));
			}
			break;
		case 43:
			//43     0 - 68  LFO 1 Destination (see destination table on page 60)
			invoke_method(mModel->lfos()->at(0), set_destination, Q_ARG(int, value));
			break;
		case 44:
			//44     0 - 160 LFO 2 Frequency (same as LFO 1)
			if(value <= 150){
				invoke_method(mModel->lfos()->at(1), set_sync_type, Q_ARG(int, 0));
				invoke_method(mModel->lfos()->at(1), set_freq, Q_ARG(int, value));
			} else {
				invoke_method(mModel->lfos()->at(1), set_sync_type, Q_ARG(int, value - 150));
			}
			break;
		case 45:
			//45     0-4     LFO 2 Shape (same as LFO 1)
			invoke_method(mModel->lfos()->at(1), set_shape, Q_ARG(int, value));
			break;
		case 46:
			//46     0 - 200 LFO 2 Amount (over 100 repeats with Key sync on)
			if(value <= 100){
				invoke_method(mModel->lfos()->at(1), set_amount, Q_ARG(int, value));
				invoke_method(mModel->lfos()->at(1), set_key_sync, Q_ARG(bool, false));
			} else {
				invoke_method(mModel->lfos()->at(1), set_key_sync, Q_ARG(bool, true));
				invoke_method(mModel->lfos()->at(1), set_amount, Q_ARG(int, value - 100));
			}
			break;
		case 47:
			//47     0 - 68  LFO 2 Destination (see destination table on page 60)
			invoke_method(mModel->lfos()->at(1), set_destination, Q_ARG(int, value));
			break;
		case 48:
			//48     0 - 198 Envelope 3 Amount; -99 to +99
			invoke_method(mModel->env3(), set_amount, Q_ARG(int, (int)value - 99));
			break;
		case 49:
			//49     0 - 68  Envelope 3 Destination (see destination table on page 60)
			invoke_method(mModel->env3(), set_destination, Q_ARG(int, value));
			break;
		case 50:
			//50     0 - 110 Envelope 3 Envelope Attack
			invoke_method(mModel->env3(), set_attack, Q_ARG(int, value));
			break;
		case 51:
			//51     0 - 110 Envelope 3 Envelope Decay
			invoke_method(mModel->env3(), set_decay, Q_ARG(int, value));
			break;
		case 52:
			//52     0 - 100 Envelope 3 Envelope Sustain
			invoke_method(mModel->env3(), set_sustain, Q_ARG(int, value));
			break;
		case 53:
			//53     0 - 110 Envelope 3 Envelope Release
			invoke_method(mModel->env3(), set_release, Q_ARG(int, value));
			break;
		case 54:
			//54     0 - 13  Trigger Select
			//                  0    All - The envelopes will be triggered by either the sequencer or MIDI notes
			//                  1    Seq – The envelopes will be triggered by the sequencer only.
			//                  2    MIDI – The envelopes will be triggered by MIDI notes only.
			//                  3    MIDI Reset– Same, with sequencer reset on Note On
			//                  4    Combo - Envelopes will only be triggered with both a
			//                       MIDI note and the sequencer is running
			//                  5    Combo Reset – same, with sequencer reset on Note On
			//                  6    External Input gates the envelopes
			//                  7    External Input gates the envelopes and resets the sequencer
			//                  8    External Input gates the sequencer
			//                  9    External Input gates the sequencer and resets the sequencer
			//                  10 Midi note plays sequence once
			//                  11 Midi note plays sequence once, resetting on multiple
			//                       notes
			//                  12   External Trigger – the sequence plays once on an
			//                       external signal
			//                  13 The sequence plays once when a key is hit
			//
			//XXX not implemented
			break;
		case 55:
			//55     0 - 73  Key Off / Transpose – 0 = MIDI notes ignored. 1 = -36
			//XXX not implemented
			break;
		case 56:
			//56     0 - 75  Sequencer 1 Destination (see destination table on page 60)
			invoke_method(mModel->sequencer(), set_destination, Q_ARG(unsigned int, 0), Q_ARG(unsigned int, value));
			break;
		case 57:
			//57     0 - 75  Sequencer 2 Destination (see destination table on page 60)
			invoke_method(mModel->sequencer(), set_destination, Q_ARG(unsigned int, 1), Q_ARG(unsigned int, value));
			break;
		case 58:
			//58     0 - 75  Sequencer 3 Destination (see destination table on page 60)
			invoke_method(mModel->sequencer(), set_destination, Q_ARG(unsigned int, 2), Q_ARG(unsigned int, value));
			break;
		case 59:
			//59     0 - 75  Sequencer 4 Destination (see destination table on page 60)
			invoke_method(mModel->sequencer(), set_destination, Q_ARG(unsigned int, 3), Q_ARG(unsigned int, value));
			break;
		case 60:
			//60     0 - 100 Noise Volume
			invoke_method(mModel->misc_audio(), set_noise_volume, Q_ARG(int, value));
			break;
		case 61:
			//61     0 - 100 External Input Volume
			invoke_method(mModel->misc_audio(), set_ext_in_volume, Q_ARG(int, value));
			break;
		case 62:
			//62     0-2     External Input Mode
			//                  0    Stereo
			//                  1    Left Input channel goes to both channels
			//                  2    Right Input channel goes to both channels
			//                  3    Left channel audio, Right channel control
			invoke_method(mModel->misc_audio(), set_ext_in_mode, Q_ARG(int, value));
			break;
		case 63:
			//63     0 - 14  Input Hack Amount
			invoke_method(mModel->misc_audio(), set_input_hack, Q_ARG(int, value));
			break;
		case 64:
			//64     0 - 200  Glide, Oscillator 1; 101 – 199 = Fingered; 200 = osc midi off
			if(value <= 100){
				invoke_method(mModel->analog_oscs()->at(0), set_glide, Q_ARG(int, value));
				invoke_method(mModel->analog_oscs()->at(0), set_glide_mode, Q_ARG(int, OscModel::normal));
			} else if (value == 200) 
				invoke_method(mModel->analog_oscs()->at(0), set_glide_mode, Q_ARG(int, OscModel::off));
			else {
				invoke_method(mModel->analog_oscs()->at(0), set_glide, Q_ARG(int, (int)value - 99));
				invoke_method(mModel->analog_oscs()->at(0), set_glide_mode, Q_ARG(int, OscModel::fingered));
			}
			break;
		case 65:
			//65     0-1      Sync; 0 = off, 1 = on
			invoke_method(mModel->analog_oscs()->at(0), set_sync, Q_ARG(bool, (bool)value));
			break;
		case 66:
			//66     30 - 250 Program tempo
			//XXX not implemented
			break;
		case 67:
			//67     0 - 12   Program Clock Divide (see Master Clock Divide for selections)
			//XXX not implemented
			break;
		case 68:
			//68     0 - 200  Glide, Oscillator 2; 101 – 199 = Fingered; 200 = osc midi off
			if(value <= 100){
				invoke_method(mModel->analog_oscs()->at(1), set_glide, Q_ARG(int, value));
				invoke_method(mModel->analog_oscs()->at(1), set_glide_mode, Q_ARG(int, OscModel::normal));
			} else if (value == 200) 
				invoke_method(mModel->analog_oscs()->at(1), set_glide_mode, Q_ARG(int, OscModel::off));
			else {
				invoke_method(mModel->analog_oscs()->at(1), set_glide, Q_ARG(int, (int)value - 99));
				invoke_method(mModel->analog_oscs()->at(1), set_glide_mode, Q_ARG(int, OscModel::fingered));
			}
			break;
		case 69:
			//69     0-5      Oscillator Slop
			invoke_method(mModel->misc_modulations(), set_osc_slop, Q_ARG(int, value));
			break;
		case 70:
			//70     0 - 12   Pitch Bend Range, in semitones
			//XXX not implemented
			break;
		case 71:
			//71     0-5      Key Mode
			//                   0    Low note priority
			//                   1    Low note priority with re-trigger
			//                   2    High note priority
			//                   3    High note priority with re-trigger
			//                   4    Last note hit priority
			//                   5    Last note hit priority with re-trigger
			//XXX not implemented
			break;
		case 72:
			//72     0 - 200 Glide, Oscillator 3; 101 – 199 = Fingered; 200 = osc midi off
			if(value <= 100){
				invoke_method(mModel->digital_oscs()->at(0), set_glide, Q_ARG(int, value));
				invoke_method(mModel->digital_oscs()->at(0), set_glide_mode, Q_ARG(int, OscModel::normal));
			} else if (value == 200) 
				invoke_method(mModel->digital_oscs()->at(0), set_glide_mode, Q_ARG(int, OscModel::off));
			else {
				invoke_method(mModel->digital_oscs()->at(0), set_glide, Q_ARG(int, (int)value - 99));
				invoke_method(mModel->digital_oscs()->at(0), set_glide_mode, Q_ARG(int, OscModel::fingered));
			}
			break;
		case 73:
			//73     0 - 100 FM, Oscillator 4 to Oscillator 3
			invoke_method(mModel->digital_oscs()->at(0), set_fm_in, Q_ARG(int, value));
			break;
		case 74:
			//74     0-4     Shape Mod Oscillator 3; 0 = Off, 1 = Sequence 1, etc.
			invoke_method(mModel->digital_oscs()->at(0), set_shape_mod, Q_ARG(int, value));
			break;
		case 75:
			//75     0 - 100 Ring Mod, Oscillator 4 to Oscillator 3
			invoke_method(mModel->digital_oscs()->at(0), set_ring_in, Q_ARG(int, value));
			break;
		case 76:
			//76     0 - 200 Glide, Oscillator 4; 101 – 199 = Fingered; 200 = osc midi off
			if(value <= 100){
				invoke_method(mModel->digital_oscs()->at(1), set_glide, Q_ARG(int, value));
				invoke_method(mModel->digital_oscs()->at(1), set_glide_mode, Q_ARG(int, OscModel::normal));
			} else if (value == 200) 
				invoke_method(mModel->digital_oscs()->at(1), set_glide_mode, Q_ARG(int, OscModel::off));
			else {
				invoke_method(mModel->digital_oscs()->at(1), set_glide, Q_ARG(int, (int)value - 99));
				invoke_method(mModel->digital_oscs()->at(1), set_glide_mode, Q_ARG(int, OscModel::fingered));
			}
			break;
		case 77:
			//77     0 - 100 FM, Oscillator 3 to Oscillator 4
			invoke_method(mModel->digital_oscs()->at(1), set_fm_in, Q_ARG(int, value));
			break;
		case 78:
			//78     0-4     Shape Mod Oscillator 4; 0 = Off, 1 = Sequence 1, etc
			invoke_method(mModel->digital_oscs()->at(1), set_shape_mod, Q_ARG(int, value));
			break;
		case 79:
			//79     0 - 100 Ring Mod, Oscillator 3 to Oscillator 4
			invoke_method(mModel->digital_oscs()->at(1), set_ring_in, Q_ARG(int, value));
			break;
		case 80:
			//80     0-1     2/4 Pole Select; 0 = 2 Pole, 1 = 4 Pole
			invoke_method(mModel->filter(), set_mode, Q_ARG(int, value));
			break;
		case 81:
			//81     0 - 100 Filter Envelope Velocity
			invoke_method(mModel->filter(), set_env_velocity, Q_ARG(int, value));
			break;
		case 82:
			//82     0 - 100 Filter Audio Modulation
			invoke_method(mModel->filter(), set_audio_mod, Q_ARG(int, value));
			break;
		case 83:
			//83     0 - 100 Filter Split
			invoke_method(mModel->filter(), set_split, Q_ARG(int, value));
			break;
		case 84:
			//84     0 – 199 Highpass Filter cutoff. 0-99 for filter on output; 100 – 199 for levels 0-99 with filter on input
			if(value < 100){
				invoke_method(mModel->filter(), set_hpf_mode, Q_ARG(int, FilterModel::after_lpf));
				invoke_method(mModel->filter(), set_hpf_cutoff, Q_ARG(int, value));
			} else {
				invoke_method(mModel->filter(), set_hpf_mode, Q_ARG(int, FilterModel::external_input));
				invoke_method(mModel->filter(), set_hpf_cutoff, Q_ARG(int, value - 100));
			}
			break;
		case 85:
			//85     0 - 24  Modulation 1 Source (see Source Table on page 62)
			invoke_method(mModel->mods()->at(0), set_source, Q_ARG(int, value));
			break;
		case 86:
			//86     0 - 198 Modulation 1 Amount; -99 to +99
			invoke_method(mModel->mods()->at(0), set_amount, Q_ARG(int, (int)value - 99));
			break;
		case 87:
			//87     0 - 68  Modulation 1 Destination (see destination table on page 60)
			invoke_method(mModel->mods()->at(0), set_destination, Q_ARG(int, value));
			break;
		case 88:
			//88     0-1     Linear/Exponential envelopes 0 = Exponential, 1 = Linear
			invoke_method(mModel->misc_audio(), set_env_curve, Q_ARG(int, value));
			break;
		case 89:
			//89     0 - 100 VCA Envelope Velocity
			invoke_method(mModel->vca(), set_velocity, Q_ARG(int, value));
			break;
		case 90:
			//90     0 - 24  Modulation 2 Source (see Source Table on page 62)
			invoke_method(mModel->mods()->at(1), set_source, Q_ARG(int, value));
			break;
		case 91:
			//91     0 - 198 Modulation 2 Amount; -99 to +99
			invoke_method(mModel->mods()->at(1), set_amount, Q_ARG(int, (int)value - 99));
			break;
		case 92:
			//92     0 - 68  Modulation 2 Destination (see destination table on page 60)
			invoke_method(mModel->mods()->at(1), set_destination, Q_ARG(int, value));
			break;
		case 93:
			//93     0 - 24  Modulation 3 Source (see Source Table on page 62)
			invoke_method(mModel->mods()->at(2), set_source, Q_ARG(int, value));
			break;
		case 94:
			//94     0 - 198 Modulation 3 Amount; -99 to +99
			invoke_method(mModel->mods()->at(2), set_amount, Q_ARG(int, (int)value - 99));
			break;
		case 95:
			//95     0 - 68  Modulation 3 Destination (see destination table on page 60)
			invoke_method(mModel->mods()->at(2), set_destination, Q_ARG(int, value));
			break;
		case 96:
			//96    0 - 24  Modulation 4 Source (see Source Table on page 62)
			invoke_method(mModel->mods()->at(3), set_source, Q_ARG(int, value));
			break;
		case 97:
			//97    0 - 198 Modulation 4 Amount; -99 to +99
			invoke_method(mModel->mods()->at(3), set_amount, Q_ARG(int, (int)value - 99));
			break;
		case 98:
			//98    0 - 68  Modulation 4 Destination (see destination table on page 60)
			invoke_method(mModel->mods()->at(3), set_destination, Q_ARG(int, value));
			break;
		case 99:
			//99    0 - 166 Delay 2 Time
			if(value <= 150) {
				invoke_method(mModel->delay(), set_delay_sync, Q_ARG(unsigned int, 1), Q_ARG(int, 0));
				invoke_method(mModel->delay(), set_delay_time, Q_ARG(unsigned int, 1), Q_ARG(int, value));
			} else {
				invoke_method(mModel->delay(), set_delay_sync, Q_ARG(unsigned int, 1), Q_ARG(int, value - 150));
			}
			break;
		case 100:
			//100    0 - 100 Delay 2 Level
			invoke_method(mModel->delay(), set_delay_level, Q_ARG(unsigned int, 1), Q_ARG(int, value));
			break;
		case 101:
			//101    0 - 166 Delay 3 Time
			if(value <= 150) {
				invoke_method(mModel->delay(), set_delay_sync, Q_ARG(unsigned int, 2), Q_ARG(int, 0));
				invoke_method(mModel->delay(), set_delay_time, Q_ARG(unsigned int, 2), Q_ARG(int, value));
			} else {
				invoke_method(mModel->delay(), set_delay_sync, Q_ARG(unsigned int, 2), Q_ARG(int, value - 150));
			}
			break;
		case 102:
			//102    0 - 100 Delay 3 Level
			invoke_method(mModel->delay(), set_delay_level, Q_ARG(unsigned int, 2), Q_ARG(int, value));
			break;
		case 103:
			//103    0 - 199 Distortion; 0-99 for distortion on output; 100 – 199 for levels 0-99 with distortion on input
			if(value < 100){
				invoke_method(mModel->misc_audio(), set_distortion_type, Q_ARG(int, MiscAudioModel::internal));
				invoke_method(mModel->misc_audio(), set_distortion_amount, Q_ARG(int, value));
			} else {
				invoke_method(mModel->misc_audio(), set_distortion_type, Q_ARG(int, MiscAudioModel::input));
				invoke_method(mModel->misc_audio(), set_distortion_amount, Q_ARG(int, value - 100));
			}
			break;
		case 104:
			//104    0 - 160 LFO 3 Frequency (same as LFO 1)
			if(value <= 150){
				invoke_method(mModel->lfos()->at(2), set_sync_type, Q_ARG(int, 0));
				invoke_method(mModel->lfos()->at(2), set_freq, Q_ARG(int, value));
			} else {
				invoke_method(mModel->lfos()->at(2), set_sync_type, Q_ARG(int, value - 150));
			}
			break;
		case 105:
			//105    0-4     LFO 3 Shape (same as LFO 1)
			invoke_method(mModel->lfos()->at(2), set_shape, Q_ARG(int, value));
			break;
		case 106:
			//106    0 - 200 LFO 3 Amount (over 100 repeats with Key sync on)
			if(value <= 100){
				invoke_method(mModel->lfos()->at(2), set_amount, Q_ARG(int, value));
				invoke_method(mModel->lfos()->at(2), set_key_sync, Q_ARG(bool, false));
			} else {
				invoke_method(mModel->lfos()->at(2), set_key_sync, Q_ARG(bool, true));
				invoke_method(mModel->lfos()->at(2), set_amount, Q_ARG(int, value - 100));
			}
			break;
		case 107:
			//107    0 - 68  LFO 3 Destination (see destination table on page 60)
			invoke_method(mModel->lfos()->at(2), set_destination, Q_ARG(int, value));
			break;
		case 108:
			//108    0 - 160 LFO 4 Frequency (same as LFO 1)
			if(value <= 150){
				invoke_method(mModel->lfos()->at(3), set_sync_type, Q_ARG(int, 0));
				invoke_method(mModel->lfos()->at(3), set_freq, Q_ARG(int, value));
			} else {
				invoke_method(mModel->lfos()->at(3), set_sync_type, Q_ARG(int, value - 150));
			}
			break;
		case 109:
			//109    0-4     LFO 4 Shape (same as LFO 1)
			invoke_method(mModel->lfos()->at(3), set_shape, Q_ARG(int, value));
			break;
		case 110:
			//110    0 - 200 LFO 4 Amount (over 100 repeats with Key sync on)
			if(value <= 100){
				invoke_method(mModel->lfos()->at(3), set_amount, Q_ARG(int, value));
				invoke_method(mModel->lfos()->at(3), set_key_sync, Q_ARG(bool, false));
			} else {
				invoke_method(mModel->lfos()->at(3), set_key_sync, Q_ARG(bool, true));
				invoke_method(mModel->lfos()->at(3), set_amount, Q_ARG(int, value - 100));
			}
			break;
		case 111:
			//111    0 - 68  LFO 4 Destination (see destination table on page 60)
			invoke_method(mModel->lfos()->at(3), set_destination, Q_ARG(int, value));
			break;
		case 112:
			//112    0 - 100 Envelope 3 Delay
			invoke_method(mModel->env3(), set_delay, Q_ARG(int, value));
			break;
		case 113:
			//113    0 - 100 Envelope 3 Velocity
			invoke_method(mModel->env3(), set_velocity, Q_ARG(int, value));
			break;
		case 114:
			//114    0 - 198 External Input Peak Amount; -99 to +99
			invoke_method(mModel->misc_modulations(), set_mod_amount, 
					Q_ARG(unsigned int, MiscModulationModel::in_peak), 
					Q_ARG(int, (int)value - 99));
			break;
		case 115:
			//115    0 - 68  External Input Peak Destination (see destination table on page 60)
			invoke_method(mModel->misc_modulations(), set_mod_destination, 
					Q_ARG(unsigned int, MiscModulationModel::in_peak), 
					Q_ARG(int, value));
			break;
		case 116:
			//116    0 - 198 External Input Envelope Follower Amount; -99 to +99
			invoke_method(mModel->misc_modulations(), set_mod_amount, 
					Q_ARG(unsigned int, MiscModulationModel::in_env_follow), 
					Q_ARG(int, (int)value - 99));
			break;
		case 117:
			//117    0 - 68  External Input Envelope Follower Destination (see destination table on page 60)
			invoke_method(mModel->misc_modulations(), set_mod_destination, 
					Q_ARG(unsigned int, MiscModulationModel::in_env_follow), 
					Q_ARG(int, value));
			break;
		case 118:
			//118    0 - 198 Velocity Amount; -99 to +99
			invoke_method(mModel->misc_modulations(), set_mod_amount, 
					Q_ARG(unsigned int, MiscModulationModel::velocity),
					Q_ARG(int, (int)value - 99));
			break;
		case 119:
			//119    0 - 68  Velocity Destination (see destination table on page 60)
			invoke_method(mModel->misc_modulations(), set_mod_destination, 
					Q_ARG(unsigned int, MiscModulationModel::velocity), 
					Q_ARG(int, value));
			break;
		case 120:
			//120    0 - 198 Mod Wheel Amount; -99 to +99
			invoke_method(mModel->misc_modulations(), set_mod_amount, 
					Q_ARG(unsigned int, MiscModulationModel::mod_wheel), 
					Q_ARG(int, (int)value - 99));
			break;
		case 121:
			//121    0 - 68  Mod Wheel Destination (see destination table on page 60)
			invoke_method(mModel->misc_modulations(), set_mod_destination, 
					Q_ARG(unsigned int, MiscModulationModel::mod_wheel), 
					Q_ARG(int, value));
			break;
		case 122:
			//122    0 - 198 Pressure Amount; -99 to +99
			invoke_method(mModel->misc_modulations(), set_mod_amount, 
					Q_ARG(unsigned int, MiscModulationModel::pressure), 
					Q_ARG(int, (int)value - 99));
			break;
		case 123:
			invoke_method(mModel->misc_modulations(), set_mod_destination, 
					Q_ARG(unsigned int, MiscModulationModel::pressure), 
					Q_ARG(int, value));
			//123    0 - 68  Pressure Destination (see destination table on page 60)
			break;
		case 124:
			//124    0 - 198 Breath Controller Amount; -99 to +99
			invoke_method(mModel->misc_modulations(), set_mod_amount, 
					Q_ARG(unsigned int, MiscModulationModel::breath), 
					Q_ARG(int, (int)value - 99));
			break;
		case 125:
			//125    0 - 68  Breath Controller Destination (see destination table on page 60)
			invoke_method(mModel->misc_modulations(), set_mod_destination, 
					Q_ARG(unsigned int, MiscModulationModel::breath), 
					Q_ARG(int, value));
			break;
		case 126:
			//126    0 - 198 Foot Controller Amount; -99 to +99
			invoke_method(mModel->misc_modulations(), set_mod_amount, 
					Q_ARG(unsigned int, MiscModulationModel::foot), 
					Q_ARG(int, (int)value - 99));
			break;
		case 127:
			//127    0 - 68  Foot Controller Destination (see destination table on page 60)
			invoke_method(mModel->misc_modulations(), set_mod_destination, 
					Q_ARG(unsigned int, MiscModulationModel::foot), 
					Q_ARG(int, value));
			break;
	}
}

void MidiDriver::update_sequence_param(uint8_t index, uint8_t value){
	uint8_t seq = index / 16;
	uint8_t step = index % 16;

	//seq zero has rests
	if(seq == 0 && value == 102){ //rest
		invoke_method(mModel->sequencer(), set_rest, Q_ARG(unsigned int, step), Q_ARG(bool, true));
	} else if (value == 101) { //length
		//if this end point is less than our current length, then set this step to be the length of our sequence
		//otherwise, set this as an endpoint for later use
		if(step < mModel->sequencer()->length(seq))
			invoke_method(mModel->sequencer(), set_length, Q_ARG(unsigned int, seq), Q_ARG(unsigned int, step));
		else
			invoke_method(mModel->sequencer(), insert_sequence_reset, Q_ARG(unsigned int, seq), Q_ARG(unsigned int, step));
	} else {
		if(seq == 0)
			invoke_method(mModel->sequencer(), set_rest, Q_ARG(unsigned int, step), Q_ARG(bool, false));
		//so if this was the reset point for the sequence, find the next reset point
		//after this step and set the length to that
		if(step == mModel->sequencer()->length(seq)){
			invoke_method(mModel->sequencer(), set_length, 
					Q_ARG(unsigned int, seq), 
					Q_ARG(unsigned int, mModel->sequencer()->first_sequence_reset(seq, step + 1)));
		}
		invoke_method(mModel->sequencer(), set_value, Q_ARG(unsigned int, seq), 
				Q_ARG(unsigned int, step), Q_ARG(unsigned int, value));
	}

}

void MidiDriver::update_main_param(uint8_t index, uint8_t value){
	switch(index){
		case 0:
			invoke_method(mModel->main(), set_program_number, Q_ARG(int, value + 1));
			break;
		case 1:
			invoke_method(mModel->main(), set_bank_number, Q_ARG(int, value + 1));
			break;
		case 2:
			invoke_method(mModel->main(), set_master_volume, Q_ARG(int, value));
			break;
		case 3:
			invoke_method(mModel->main(), set_master_transpose, Q_ARG(int, value - 36));
			break;
		case 4:
			invoke_method(mModel->main(), set_bpm, Q_ARG(int, value));
			break;
		case 5:
			invoke_method(mModel->main(), set_clock_divide, Q_ARG(int, value));
			break;
		case 6:
			invoke_method(mModel->main(), set_use_program_tempo, Q_ARG(bool, value));
			break;
		case 7:
			invoke_method(mModel->main(), set_midi_clock_select, Q_ARG(int, value));
			break;
		case 8:
			invoke_method(mModel->main(), set_lock_sequence, Q_ARG(bool, value));
			break;
		case 9:
			invoke_method(mModel->main(), set_poly_chain_select, Q_ARG(int, value));
			break;
		case 10:
			invoke_method(mModel->main(), set_input_gain, Q_ARG(int, value));
			break;
		case 11:
			invoke_method(mModel->main(), set_master_fine_tune, Q_ARG(int, value - 50));
			break;
		case 12:
			invoke_method(mModel->main(), set_midi_receive, Q_ARG(int, value));
			break;
		case 13:
			invoke_method(mModel->main(), set_midi_transmit, Q_ARG(int, value));
			break;
		case 14:
			invoke_method(mModel->main(), set_midi_channel, Q_ARG(int, value));
			break;
		default:
			break;
	};
}

void MidiDriver::send_program_param(uint8_t index, uint8_t value){
	if(mMidiOut){
		uint8_t msg[10];
		msg[0] = (uint8_t)MIDI_SYSEX_START;
		memcpy(msg + 1, evolver_sysex_header, 3);
		msg[4] = prog_param;
		msg[5] = index & 0x7F;
		msg[6] = value & 0x0F;
		msg[7] = (value >> 4) & 0x0F;
		msg[8] = (uint8_t)MIDI_SYSEX_END;
		Pm_WriteSysEx(mMidiOut, 0, msg);
	}
}

void MidiDriver::send_sequencer_param(uint8_t step, uint8_t value){
	if(mMidiOut){
		uint8_t msg[10];
		msg[0] = (uint8_t)MIDI_SYSEX_START;
		memcpy(msg + 1, evolver_sysex_header, 3);
		msg[4] = seq_param;
		msg[5] = step & 0x7F;
		msg[6] = value & 0x0F;
		msg[7] = (value >> 4) & 0x0F;
		msg[8] = (uint8_t)MIDI_SYSEX_END;
		Pm_WriteSysEx(mMidiOut, 0, msg);
	}
}

void MidiDriver::send_main_param(uint8_t index, uint8_t value){
	if(mMidiOut){
		uint8_t msg[10];
		msg[0] = (uint8_t)MIDI_SYSEX_START;
		memcpy(msg + 1, evolver_sysex_header, 3);
		msg[4] = main_param;
		msg[5] = index & 0x7F;
		msg[6] = value & 0x0F;
		msg[7] = (value >> 4) & 0x0F;
		msg[8] = (uint8_t)MIDI_SYSEX_END;
		Pm_WriteSysEx(mMidiOut, 0, msg);
	}
}

void MidiDriver::update_device_list(){
	mInputMap.clear();
	mOutputMap.clear();
	/* list device information */
	for (int i = 0; i < Pm_CountDevices(); i++) {
        const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
        //printf("%d: %s, %s", i, info->interf, info->name);
        if (info->input)
			  mInputMap[(unsigned int) i] = QString(info->name);
        if (info->output)
			  mOutputMap[(unsigned int) i] = QString(info->name);
    }
}

//unpack our data
void MidiDriver::unpack_data(std::vector<uint8_t> packed, std::vector<uint8_t> &unpacked){
	uint8_t ms_bits = 0;
	for(unsigned int i = 0; i < packed.size(); i++){
		if(i % 8 == 0){
			ms_bits = packed[i];
		} else {
			unsigned int bit_position = (i % 8) - 1;
			unpacked.push_back(packed[i] | (((ms_bits >> bit_position) & 0x1)) << 7);
		}
	}
}

