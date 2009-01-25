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

#ifndef ENV3_HPP
#define ENV3_HPP

#include "model.hpp"

class Env3Model : public Model {
	Q_OBJECT
	public:
		Env3Model(QObject * parent = NULL);
		static const unsigned int delay_max;
		static const int amount_min;
		static const int amount_max;
		static const unsigned int attack_max;
		static const unsigned int decay_max;
		static const unsigned int sustain_max;
		static const unsigned int release_max;
		static const unsigned int velocity_max;
	public slots:
		void set_destination(int val);
		void set_delay(int val);
		void set_amount(int val);
		void set_attack(int val);
		void set_decay(int val);
		void set_sustain(int val);
		void set_release(int val);
		void set_velocity(int val);
	signals:
		void destination_changed(int val);
		void delay_changed(int val);
		void amount_changed(int val);
		void attack_changed(int val);
		void decay_changed(int val);
		void sustain_changed(int val);
		void release_changed(int val);
		void velocity_changed(int val);
	private:
		unsigned int mDestination;
		unsigned int mDelay;
		int mAmount;
		unsigned int mAttack;
		unsigned int mDecay;
		unsigned int mSustain;
		unsigned int mRelease;
		unsigned int mVelocity;
};

#include <QWidget>
class QComboBox;
class SliderSpinBox;
class QGridLayout;
class ModDestComboBox;

class Env3View : public QWidget {
	Q_OBJECT
	public:
		Env3View(QWidget * parent = NULL);
		void connect_to_model(Env3Model * model);
	public slots:
		void set_destination(int val);
		void set_delay(int val);
		void set_amount(int val);
		void set_attack(int val);
		void set_decay(int val);
		void set_sustain(int val);
		void set_release(int val);
		void set_velocity(int val);
	signals:
		void destination_changed(int val);
		void delay_changed(int val);
		void amount_changed(int val);
		void attack_changed(int val);
		void decay_changed(int val);
		void sustain_changed(int val);
		void release_changed(int val);
		void velocity_changed(int val);
	private:
		QGridLayout * mLayout;

		ModDestComboBox * mDestination;

		SliderSpinBox * mDelay;
		SliderSpinBox * mAmount;
		SliderSpinBox * mAttack;
		SliderSpinBox * mDecay;
		SliderSpinBox * mSustain;
		SliderSpinBox * mRelease;
		SliderSpinBox * mVelocity;
};

#endif
