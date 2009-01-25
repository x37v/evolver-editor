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
};

#endif
