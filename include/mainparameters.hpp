#ifndef MAIN_PARAMETERS_HPP
#define MAIN_PARAMETERS_HPP

#include "model.hpp"

class MainModel : public Model {
	Q_OBJECT
	public:
		MainModel(QObject * parent = NULL);
		static const unsigned int program_number_max;
		static const unsigned int program_number_min;
		static const unsigned int bank_number_max;
		static const unsigned int bank_number_min;
		static const unsigned int master_volume_max;
		static const int master_transpose_max;
		static const int master_transpose_min;
		static const unsigned int bpm_max;
		static const unsigned int bpm_min;
		static const unsigned int clock_divide_max;
		static const unsigned int midi_clock_select_max;
		static const unsigned int poly_chain_select_max;
		static const unsigned int input_gain_max;
		static const int master_fine_tune_max;
		static const int master_fine_tune_min;
		static const unsigned int midi_receive_max;
		static const unsigned int midi_transmit_max;
		static const unsigned int midi_channel_max;

		static const char * clock_divide_names[];
		static const char * midi_clock_select_names[];
		static const char * poly_chain_select_names[];
		static const char * input_gain_names[];
		static const char * midi_receive_names[];
		static const char * midi_transmit_names[];
	public slots:
		void set_program_number(int val);
		void set_bank_number(int val);
		void set_master_volume(int val);
		void set_master_transpose(int val);
		void set_bpm(int val);
		void set_clock_divide(int val);
		void set_use_program_tempo(bool val);
		void set_midi_clock_select(int val);
		void set_lock_sequence(bool val);
		void set_poly_chain_select(int val);
		void set_input_gain(int val);
		void set_master_fine_tune(int val);
		void set_midi_receive(int val);
		void set_midi_transmit(int val);
		void set_midi_channel(int val);
	signals:
		void program_number_changed(int val);
		void bank_number_changed(int val);
		void master_volume_changed(int val);
		void master_transpose_changed(int val);
		void bpm_changed(int val);
		void clock_divide_changed(int val);
		void use_program_tempo_changed(bool val);
		void midi_clock_select_changed(int val);
		void lock_sequence_changed(bool val);
		void poly_chain_select_changed(int val);
		void input_gain_changed(int val);
		void master_fine_tune_changed(int val);
		void midi_receive_changed(int val);
		void midi_transmit_changed(int val);
		void midi_channel_changed(int val);
	private:
		unsigned int mProgramNumber;
		unsigned int mBankNumber;
		unsigned int mMasterVolume;
		int mMasterTranspose;
		unsigned int mBPM;
		unsigned int mClockDiv;
		bool mUseProgramTempo;
		unsigned int mMIDIClockSelect;
		bool mLockSequence;
		unsigned int mPolyChainSelect;
		unsigned int mInputGain;
		int mMasterFineTune;
		unsigned int mMIDIReceive;
		unsigned int mMIDITransmit;
		unsigned int mMIDIChannel;
};

#include <QWidget>
class SliderSpinBox;
class QComboBox;
class QToolButton;
class QSpinBox;
class QGridLayout;

class MainView : public QWidget {
	Q_OBJECT
	public:
		MainView(QWidget * parent = NULL);
		void connect_to_model(MainModel * model);
	private:
		QGridLayout * mLayout;

		QSpinBox * mProgramNumber;
		QSpinBox * mBankNumber;
		SliderSpinBox * mMasterVolume;
		SliderSpinBox * mMasterTranspose;
		SliderSpinBox * mBPM;
		QComboBox * mClockDiv;
		QToolButton * mUseProgramTempo;
		QComboBox * mMIDIClockSelect;
		QToolButton * mLockSequence;
		QComboBox * mPolyChainSelect;
		QComboBox * mInputGain;
		SliderSpinBox * mMasterFineTune;
		QComboBox * mMIDIReceive;
		QComboBox * mMIDITransmit;
		QComboBox * mMIDIChannel;
	public slots:
		void set_program_number(int val);
		void set_bank_number(int val);
		void set_master_volume(int val);
		void set_master_transpose(int val);
		void set_bpm(int val);
		void set_clock_divide(int val);
		void set_use_program_tempo(bool val);
		void set_midi_clock_select(int val);
		void set_lock_sequence(bool val);
		void set_poly_chain_select(int val);
		void set_input_gain(int val);
		void set_master_fine_tune(int val);
		void set_midi_receive(int val);
		void set_midi_transmit(int val);
		void set_midi_channel(int val);
	signals:
		void program_number_changed(int val);
		void bank_number_changed(int val);
		void master_volume_changed(int val);
		void master_transpose_changed(int val);
		void bpm_changed(int val);
		void clock_divide_changed(int val);
		void use_program_tempo_changed(bool val);
		void midi_clock_select_changed(int val);
		void lock_sequence_changed(bool val);
		void poly_chain_select_changed(int val);
		void input_gain_changed(int val);
		void master_fine_tune_changed(int val);
		void midi_receive_changed(int val);
		void midi_transmit_changed(int val);
		void midi_channel_changed(int val);
};

#endif
