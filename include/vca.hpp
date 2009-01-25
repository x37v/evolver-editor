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

#ifndef VCA_HPP
#define VCA_HPP

#include "model.hpp"

class VCAModel : public Model {
	Q_OBJECT
	public:
		VCAModel(QObject * parent = NULL);
		static const unsigned int level_max;
		static const unsigned int env_amount_max;
		static const unsigned int attack_max;
		static const unsigned int decay_max;
		static const unsigned int sustain_max;
		static const unsigned int release_max;
		static const unsigned int velocity_max;
		static const char * pan_type_names[];
		static const unsigned int num_pan_types;
		enum pan_type {
			stereo_one,
			stereo_two,
			stereo_three,
			mono,
			rev_stereo_one,
			rev_stereo_two,
			rev_stereo_three
		};
	public slots:
		void set_level(int val);
		void set_env_amount(int val);
		void set_attack(int val);
		void set_decay(int val);
		void set_sustain(int val);
		void set_release(int val);
		void set_velocity(int val);
		void set_pan(int val);
	signals:
		void level_changed(int val);
		void env_amount_changed(int val);
		void attack_changed(int val);
		void decay_changed(int val);
		void sustain_changed(int val);
		void release_changed(int val);
		void velocity_changed(int val);
		void pan_changed(int val);
	private:
		unsigned int mLevel;
		unsigned int mEnvAmount;
		unsigned int mAttack;
		unsigned int mDecay;
		unsigned int mSustain;
		unsigned int mRelease;
		unsigned int mVelocity;
		pan_type mPanType;
};

#include <QWidget>
class QComboBox;
class SliderSpinBox;
class QGridLayout;

class VCAView : public QWidget {
	Q_OBJECT
	public:
		VCAView(QWidget * parent = NULL);
		void connect_to_model(VCAModel * model);
	private:
		QGridLayout * mLayout;

		SliderSpinBox * mLevel;
		SliderSpinBox * mEnvAmount;
		SliderSpinBox * mAttack;
		SliderSpinBox * mDecay;
		SliderSpinBox * mSustain;
		SliderSpinBox * mRelease;
		SliderSpinBox * mVelocity;
		QComboBox * mPanType;
	public slots:
		void set_level(int val);
		void set_env_amount(int val);
		void set_attack(int val);
		void set_decay(int val);
		void set_sustain(int val);
		void set_release(int val);
		void set_velocity(int val);
		void set_pan(int val);
	signals:
		void level_changed(int val);
		void env_amount_changed(int val);
		void attack_changed(int val);
		void decay_changed(int val);
		void sustain_changed(int val);
		void release_changed(int val);
		void velocity_changed(int val);
		void pan_changed(int val);
};

#endif
