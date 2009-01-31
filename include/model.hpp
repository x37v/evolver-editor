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

#ifndef MODEL_HPP
#define MODEL_HPP

#include <QObject>

class MidiDriver;

class Model : public QObject {
	Q_OBJECT
	public:
		Model(QObject * parent = NULL);
		static void set_midi_driver(MidiDriver * driver);
	protected:
		static MidiDriver * cMidiDriver;
		void send_program_param(uint8_t index, uint8_t value);
		void send_sequencer_param(uint8_t step, uint8_t value);
};

#endif
