#ifndef MIDI_DRIVER_VIEW_HPP
#define MIDI_DRIVER_VIEW_HPP

//this class gives the user the ability to select the midi Input and output
//devices with a GUI

#include <QWidget>
#include <QString>
#include <map>

class MidiDriver;
class QComboBox;
class QGridLayout;

class MidiDriverView : public QWidget {
	Q_OBJECT
	public:
		MidiDriverView(QWidget * parent = NULL);
		void update_inputs(const std::map<unsigned int, QString> * inputs);
		void update_outputs(const std::map<unsigned int, QString> * outputs);
	signals:
		void input_index_changed(int);
		void output_index_changed(int);
	public slots:
		void set_input_index(int index);
		void set_output_index(int index);
	private slots:
		void set_current_input_index(int index);
		void set_current_output_index(int index);
	private:
		QComboBox * mInputSelect;
		QComboBox * mOutputSelect;
		QGridLayout * mLayout;
};

#endif
