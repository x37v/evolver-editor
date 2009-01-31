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

#ifndef MIDI_DRIVER_HPP
#define MIDI_DRIVER_HPP

#include <QThread>
#include "portmidi.h"
#include "porttime.h"
#include <vector>
#include <map>
#include <QString>

class ApplicationModel;
class QTimer;

class MidiDriver : public QThread {
	Q_OBJECT
	public:
		MidiDriver(ApplicationModel * model, QObject * parent = NULL);
		virtual ~MidiDriver();
		void open_input(int index);
		void open_output(int index);
		void open(int input, int output);
		void close_input();
		void close_output();
		void close();
		void run();
		const std::map<unsigned int, QString> * input_map();
		const std::map<unsigned int, QString> * output_map();
	protected:
		void update_model_param(uint8_t index, uint8_t value);
		void unpack_data(std::vector<uint8_t> packed, std::vector<uint8_t> &unpacked);
		static const uint8_t evolver_sysex_header[];
	public slots:
		void request_edit_buffer();
		void request_waveform_dump(int index);
		void send_program_param(uint8_t index, uint8_t value);
		void send_sequencer_param(uint8_t step, uint8_t value);
		void update_device_list();
	protected slots:
		void poll();
	private:
		enum command_types {
			prog_param = 0x01,
			prog_dump = 0x02,
			edit_dump = 0x03,
			edit_dump_request = 0x06,
			seq_param = 0x08,
			main_param = 0x09,
			wave_dump = 0x0a,
			wave_dump_request = 0x0b,
			main_param_dump = 0x0f,
			prog_name_dump = 0x11
		};
		command_types mCurrentCommand;
		PmStream *mMidiIn;
		PmStream *mMidiOut;
		bool mReading;
		unsigned int mReadingCount;
		std::vector<uint8_t> mInputBuffer;
		ApplicationModel * mModel;
		//the current param we're working with
		unsigned int mInputParamNumber;
		uint8_t mInputParamValue;
		QTimer * mTimer;
		std::map<unsigned int, QString> mInputMap;
		std::map<unsigned int, QString> mOutputMap;
};

#endif
