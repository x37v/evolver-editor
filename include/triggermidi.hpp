#ifndef TRIGGER_MIDI_HPP
#define TRIGGER_MIDI_HPP

#include "model.hpp"

//this class implements both the trigger and misc midi program settings
class TriggerMIDIModel : public Model {
	Q_OBJECT
	public:
		TriggerMIDIModel(QObject * parent);
		static const unsigned int trigger_select_max;
		static const int midi_transpose_max;
		static const int midi_transpose_min;
		static const unsigned int midi_key_mode_max;
		static const unsigned int midi_pitch_bend_range_max;

		static const char * trigger_select_names[];
		static const char * midi_key_mode_names[];
	public slots:
		void set_trigger_select(int val);
		void set_midi_transpose(int val);
		void set_midi_keys_on(bool val);
		void set_midi_key_mode(int val);
		void set_midi_pitch_bend_range(int val);
	signals:
		void trigger_select_changed(int val);
		void midi_transpose_changed(int val);
		void midi_keys_on_changed(bool val);
		void midi_key_mode_changed(int val);
		void midi_pitch_bend_range_changed(int val);
	private:
		unsigned int mTriggerSelect;
		int mMIDITranspose;
		bool mMIDIKeysOn;
		unsigned int mMIDIKeyMode;
		unsigned int mMIDIPitchBendRange;
};

#include <QWidget>

class SliderSpinBox;
class QComboBox;
class QToolButton;
class QGridLayout;

class TriggerMIDIView : public QWidget {
	Q_OBJECT
	public:
		TriggerMIDIView(QWidget * parent = NULL);
		void connect_to_model(TriggerMIDIModel * model);
	public slots:
		void set_trigger_select(int val);
		void set_midi_transpose(int val);
		void set_midi_keys_on(bool val);
		void set_midi_key_mode(int val);
		void set_midi_pitch_bend_range(int val);
	signals:
		void trigger_select_changed(int val);
		void midi_transpose_changed(int val);
		void midi_keys_on_changed(bool val);
		void midi_key_mode_changed(int val);
		void midi_pitch_bend_range_changed(int val);
	private:
		QGridLayout * mLayout;
		QComboBox * mTriggerSelect;
		SliderSpinBox * mMIDITranspose;
		QToolButton * mMIDIKeysOn;
		QComboBox * mMIDIKeyMode;
		SliderSpinBox * mMIDIPitchBendRange;
};

#endif
