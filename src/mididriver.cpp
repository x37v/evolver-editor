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
#include <QTimer>

#include <iostream>
using std::endl;
using std::cout;

#define is_real_time_msg(msg) ((0xF0 & Pm_MessageStatus(msg)) == 0xF8)
#define MIDI_SYSEX_START 0xF0
#define MIDI_SYSEX_END 0xF7

#define invoke_method(ob, meth, ...) (QMetaObject::invokeMethod(ob, #meth, Qt::QueuedConnection, __VA_ARGS__))
#define invoke_method2(ob, meth, arg0, arg1) (QMetaObject::invokeMethod(ob, #meth, Qt::QueuedConnection, arg0, arg1))

MidiDriver::MidiDriver(ApplicationModel * model, QObject * parent) : QThread(parent){
	mMidiOut = NULL;
	mMidiIn = NULL;
	mReading = false;
	mModel = model;
	mTimer = new QTimer;
	connect(mTimer, SIGNAL(timeout()), this, SLOT(poll()));

	/* list device information */
	for (int i = 0; i < Pm_CountDevices(); i++) {
        const PmDeviceInfo *info = Pm_GetDeviceInfo(i);
        printf("%d: %s, %s", i, info->interf, info->name);
        if (info->input) printf(" (input)");
        if (info->output) printf(" (output)");
        printf("\n");
    }
	setTerminationEnabled();
}

void MidiDriver::run(){
	if(mMidiIn){
		//every 1 ms
		mTimer->start(1);
		exec();
	}
}

void MidiDriver::poll(){
	PmEvent msg;
	PmError count;
	count = Pm_Read(mMidiIn, &msg, 1);

	if (count == 0) 
		return;
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
			//XXX do something!
			mReading = false;
		} else if (mReading){
			switch(mReadingCount){
				case 0:
					//DSI Id
					if(data != 0x01)
						mReading = false;
					break;
				case 1:
					//Evolver Id
					if(data != 0x20)
						mReading = false;
					break;
				case 2:
					//File Version
					if(data != 0x01)
						mReading = false;
					break;
				case 3:
					//command
					switch(data){
						case prog_param:
							mCurrentCommand = prog_param;
							break;
						default:
							mReading = false;
					};
					break;
				default:
					//after the header, this is our b.s.
					unsigned int index = mReadingCount - 4;
					if(mCurrentCommand == prog_param){
						switch(index){
							case 0:
								mParamNumber = data;
								break;
							case 1:
								mParamValue = data;
								break;
							case 2:
								mParamValue |= (data << 4);
								//update the shit
								update_model_param(mParamNumber, mParamValue);
								break;
							default:
								break;
						}
					}
					break;
			};
			//increment the count
			mReadingCount++;
		}
	}
}

void MidiDriver::open_input(int index){
	close_input();
	Pm_OpenInput(&mMidiIn, index, NULL, 512, NULL, NULL);
}

void MidiDriver::open_output(int index){
	close_output();
	Pm_OpenOutput(&mMidiOut, index, NULL, 512, NULL, NULL, 0);
}

void MidiDriver::open(int input, int output){
	open_input(input);
	open_output(output);
}

void MidiDriver::close_input(){
	if(mMidiIn)
		Pm_Close(mMidiIn);
}

void MidiDriver::close_output(){
	if(mMidiOut)
		Pm_Close(mMidiOut);
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

void MidiDriver::update_model_param(uint8_t index, uint8_t value){
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
			invoke_method(mModel->digital_oscs()->at(0), set_shape, Q_ARG(int, value));
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
			invoke_method(mModel->digital_oscs()->at(1), set_shape, Q_ARG(int, value));
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
			//XXX not implemented
			break;
		case 57:
			//57     0 - 75  Sequencer 2 Destination (see destination table on page 60)
			//XXX not implemented
			break;
		case 58:
			//58     0 - 75  Sequencer 3 Destination (see destination table on page 60)
			//XXX not implemented
			break;
		case 59:
			//59     0 - 75  Sequencer 4 Destination (see destination table on page 60)
			//XXX not implemented
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
