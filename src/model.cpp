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
