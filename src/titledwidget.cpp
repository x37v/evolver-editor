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

#include "titledwidget.hpp"
#include <QLabel>
#include <QVBoxLayout>

TitledWidget::TitledWidget(QString title, QWidget * widget, QWidget * parent) : QWidget(parent){
	mLayout = new QVBoxLayout(this);
	mLabel = new QLabel(title, this);

	mWidget = widget;
	widget->setParent(this);
	mLayout->addWidget(mLabel, 0, Qt::AlignHCenter);
	mLayout->addWidget(mWidget, 0);
	mLayout->setContentsMargins(1,1,1,1);
	mLayout->setSpacing(2);
	mLayout->addStretch(1);

	setLayout(mLayout);
}

