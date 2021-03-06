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

#ifndef TITLED_WIDGET_HPP
#define TITLED_WIDGET_HPP

#include <QWidget>
#include <QString>

class QVBoxLayout;
class QLabel;

//this widget is simply another widget with a QLabel at the top

class TitledWidget : public QWidget {
	Q_OBJECT
	public:
		TitledWidget(QString title, QWidget * widget, QWidget * parent = NULL);
	private:
		QWidget * mWidget;
		QLabel * mLabel;
		QVBoxLayout * mLayout;
};
#endif
