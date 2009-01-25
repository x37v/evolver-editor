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

#ifndef APPLICATION_VIEW_HPP
#define APPLICATION_VIEW_HPP

#include <QWidget>

class QTabWidget;
class AudioAndEnvelopeView;
class ModulationView;
class QVBoxLayout;
class ApplicationModel;

class ApplicationView : public QWidget {
	Q_OBJECT
	public:
		ApplicationView(QWidget * parent = NULL);
		AudioAndEnvelopeView * audio_and_envelopes();
		ModulationView * modulation();
		void connect_to_model(ApplicationModel * model);
	private:
		QVBoxLayout * mLayout;
		QTabWidget * mTabView;

		AudioAndEnvelopeView * mAudioAndEnvelopes;
		ModulationView * mModulations;
};

#endif

