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

#ifndef MODULATION_ROUTING_HPP
#define MODULATION_ROUTING_HPP

#include <QWidget>
class QLabel;
class QComboBox;
class SliderSpinBox;
class QGridLayout;
class ModDestComboBox;

class ModRoutingModel : public QObject {
	Q_OBJECT
	public:
		ModRoutingModel(QObject * parent = NULL);
		const static int amount_max;
		const static int amount_min;
		int amount();
		unsigned int source();
		unsigned int destination();
	signals:
		void amount_changed(int);
		void destination_changed(int);
		void source_changed(int);
	public slots:
		void set_amount(int amount);
		void set_source(int source);
		void set_destination(int dest);
	private:
		int mAmount;
		unsigned int mSource;
		unsigned int mDestination;
};

class ModRoutingView : public QWidget {
	Q_OBJECT
	public:
		ModRoutingView(QWidget * parent = NULL);
		std::vector<QLabel *> * labels();
		void connect_to_model(ModRoutingModel * model);
	signals:
		void amount_changed(int);
		void destination_changed(int);
		void source_changed(int);
	public slots:
		void show_labels(bool show);
		void set_amount(int amount);
		void set_source(int source);
		void set_destination(int dest);
	private:
		std::vector<QLabel *> mLabels;
		SliderSpinBox * mAmountSlider;
		QComboBox * mSrcSelect;
		ModDestComboBox * mDestSelect;
		QGridLayout * mLayout;

};

#endif
