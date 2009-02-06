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

#include "model.hpp"
#include "mididriver.hpp"

MidiDriver * Model::cMidiDriver = NULL;

Model::Model(QObject * parent) : QObject(parent) {
}

void Model::set_midi_driver(MidiDriver * driver){
	cMidiDriver = driver;
}

void Model::send_program_param(uint8_t index, uint8_t value){
	if(cMidiDriver)
		cMidiDriver->send_program_param(index, value);
}

void Model::send_sequencer_param(uint8_t step, uint8_t value){
	if(cMidiDriver)
		cMidiDriver->send_sequencer_param(step, value);
}

void Model::send_main_param(uint8_t index, uint8_t value){
	if(cMidiDriver)
		cMidiDriver->send_main_param(index, value);
}

