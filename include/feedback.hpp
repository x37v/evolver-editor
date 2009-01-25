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

#ifndef FEEDBACK_HPP
#define FEEDBACK_HPP

#include "model.hpp"

class FeedbackModel : public Model {
	Q_OBJECT
	public:
		FeedbackModel(QObject * parent = NULL);
		static const unsigned int freq_max;
		static const unsigned int level_max;
	public slots:
		void set_freq(int f);
		void set_level(int l);
		void set_grunge(bool g);
	signals:
		void freq_changed(int);
		void level_changed(int);
		void grunge_changed(bool);
	private:
		unsigned int mFreq;
		unsigned int mLevel;
		bool mGrunge;
};

#include <QWidget>
class QPushButton;
class QGridLayout;
class SliderSpinBox;

class FeedbackView : public QWidget {
	Q_OBJECT
	public:
		FeedbackView(QWidget * parent = NULL);
		void connect_to_model(FeedbackModel * model);
	public slots:
		void set_freq(int f);
		void set_level(int l);
		void set_grunge(bool g);
	signals:
		void freq_changed(int);
		void level_changed(int);
		void grunge_changed(bool);
	private:
		QGridLayout * mLayout;

		SliderSpinBox * mFreq;
		SliderSpinBox * mLevel;
		QPushButton * mGrunge;
};

#endif
