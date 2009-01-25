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

#ifndef MISC_MODULATION_HPP
#define MISC_MODULATION_HPP

#include "model.hpp"
#include <vector>

class MiscModulationModel : public Model {
	Q_OBJECT
	public:
		MiscModulationModel(QObject * parent = NULL);
		enum modulation_source {
			in_peak,
			in_env_follow,
			velocity,
			mod_wheel,
			pressure,
			breath,
			foot
		};
		const static char * modulation_source_names[];
		const static unsigned int num_modulation_sources;
		const static unsigned int osc_slop_max;
		const static int amount_min;
		const static int amount_max;
	public slots:
		void set_osc_slop(int value);
		void set_mod_amount(unsigned int index, int value);
		void set_mod_destination(unsigned int index, int value);
	signals:
		void osc_slop_changed(int);
		void mod_amount_changed(unsigned int index, int value);
		void mod_destination_changed(unsigned int index, int value);
	private:
		unsigned int mOscSlop;

		std::vector<int> mAmounts;
		std::vector<unsigned int> mDestinations;
};

#include <QWidget>
class QComboBox;
class SliderSpinBox;
class QGridLayout;
class ModDestComboBox;

class MiscModulationView : public QWidget {
	Q_OBJECT
	public:
		MiscModulationView(QWidget * parent = NULL);
		void connect_to_model(MiscModulationModel * model);
	private slots:
		void update_mod_amount(int index);
		void update_mod_dest(int index);
	public slots:
		void set_osc_slop(int value);
		void set_mod_amount(unsigned int index, int value);
		void set_mod_destination(unsigned int index, int value);
	signals:
		void osc_slop_changed(int);
		void mod_amount_changed(unsigned int index, int value);
		void mod_destination_changed(unsigned int index, int value);
	private:
		QGridLayout * mLayout;
		SliderSpinBox * mOscSlop;

		std::vector<SliderSpinBox *> mAmounts;
		std::vector<ModDestComboBox *> mDestinations;
};

#endif
