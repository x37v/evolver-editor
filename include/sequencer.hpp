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

#ifndef SEQUENCER_HPP
#define SEQUENCER_HPP

#include "model.hpp"


class SequencerModel : public Model {
	Q_OBJECT
	public:
		SequencerModel(QObject * parent = NULL);
		unsigned int length(unsigned int seq);
		//find the lowest sequence end point
		unsigned int first_sequence_reset(unsigned int seq);
	signals:
		void length_changed(unsigned int seq, unsigned int length);
		void rest_changed(unsigned int step, bool on);
		void value_changed(unsigned int seq, unsigned int step, unsigned int value);
		void destination_changed(unsigned int seq, unsigned int index);
	public slots:
		void set_length(unsigned int seq, unsigned int length);
		void set_rest(unsigned int step, bool rest);
		void set_value(unsigned int seq, unsigned int step, unsigned int value);
		void set_destination(unsigned int seq, unsigned int index);
		void insert_sequence_reset(unsigned int seq, unsigned int step);
		void remove_sequence_reset(unsigned int seq, unsigned int step);
		void clear_sequence_resets();
	private:
		//values for each sequence at each step
		unsigned int mSequence[4][16];
		//is this an end point for a sequence
		bool mSequenceReset[4][16];
		//the length of each sequence [0..16]
		unsigned int mLength[4];
		//is this step a rest
		bool mRest[16];
		unsigned int mDestination[4];
};

#include <QWidget>
class SliderSpinBox;
class QSpinBox;
class QGridLayout;
class QToolButton;
class ModDestComboBox;

class SequencerView : public QWidget {
	Q_OBJECT
	public:
		SequencerView(QWidget * parent = NULL);
		void connect_to_model(SequencerModel * model);
	signals:
		void length_changed(unsigned int seq, unsigned int length);
		void rest_changed(unsigned int step, bool on);
		void value_changed(unsigned int seq, unsigned int step, unsigned int value);
		void destination_changed(unsigned int seq, unsigned int index);
	public slots:
		void set_length(unsigned int seq, unsigned int length);
		void set_rest(unsigned int step, bool rest);
		void set_value(unsigned int seq, unsigned int step, unsigned int value);
		void set_destination(unsigned int seq, unsigned int index);
	protected slots:
		void local_set_length(int index);
		void local_set_rest(int index);
		void local_set_value(int index);
		void local_set_destination(int index);
	private:
		QToolButton * mRest[16];
		QGridLayout * mLayout;
		SliderSpinBox * mSequence[4][16];
		QSpinBox * mLength[4];
		ModDestComboBox * mDestination[4];
};


#endif
