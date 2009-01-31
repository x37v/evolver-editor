#ifndef SEQUENCER_HPP
#define SEQUENCER_HPP

#include "model.hpp"


class SequencerModel : public Model {
	Q_OBJECT
	public:
		SequencerModel(QObject * parent = NULL);
	signals:
		void length_changed(unsigned int seq, unsigned int length);
		void rest_changed(unsigned int step, bool on);
		void value_changed(unsigned int seq, unsigned int step, unsigned int value);
	public slots:
		void set_length(unsigned int seq, unsigned int length);
		void set_rest(unsigned int step, bool rest);
		void set_value(unsigned int seq, unsigned int step, unsigned int value);
	private:
		//values for each sequence at each step
		unsigned int mSequence[4][16];
		//the length of each sequence [0..16]
		unsigned int mLength[4];
		//is this step a rest
		bool mRest[16];
};

#include <QWidget>
class SliderSpinBox;
class QSpinBox;
class QGridLayout;
class QToolButton;

class SequencerView : public QWidget {
	Q_OBJECT
	public:
		SequencerView(QWidget * parent = NULL);
	signals:
		void length_changed(unsigned int seq, unsigned int length);
		void rest_changed(unsigned int step, bool on);
		void value_changed(unsigned int seq, unsigned int step, unsigned int value);
	public slots:
		void set_length(unsigned int seq, unsigned int length);
		void set_rest(unsigned int step, bool rest);
		void set_value(unsigned int seq, unsigned int step, unsigned int value);
	private:
		QToolButton * mRest[16];
		QGridLayout * mLayout;
		SliderSpinBox * mSequence[4][16];
		QSpinBox * mLength[4];
};


#endif
