#ifndef MIDI_DRIVER_HPP
#define MIDI_DRIVER_HPP

#include <QThread>
#include "portmidi.h"
#include "porttime.h"
#include <vector>

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
	protected:
		void update_model_param(uint8_t index, uint8_t value);
	protected slots:
		void poll();
	private:
		enum command_types {
			prog_param = 0x01,
			prog_dump = 0x02,
			edit_dump = 0x03,
			wave_dump = 0x0a,
			seq_param = 0x08,
			main_param = 0x09,
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
		unsigned int mParamNumber;
		uint8_t mParamValue;
		QTimer * mTimer;
};

#endif
