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

#ifndef AUDIO_AND_ENVELOPE_VIEW_HPP
#define AUDIO_AND_ENVELOPE_VIEW_HPP

#include <QWidget>
#include <vector>

class AnalogOscView;
class DigitalOscView;
class DelayView;
class FilterView;
class VCAView;
class Env3View;
class FeedbackView;
class MiscAudioView;
class ApplicationModel;

//this is a view/window which include the audio an envelope control settings

class AudioAndEnvelopeView : public QWidget {
	Q_OBJECT
	public:
		AudioAndEnvelopeView(QWidget * parent = NULL);
		std::vector<AnalogOscView *> * analog_oscs();
		std::vector<DigitalOscView *> * digital_oscs();
		DelayView * delay();
		FilterView * filter();
		VCAView * vca();
		Env3View * env3();
		FeedbackView * feedback();
		MiscAudioView * misc_audio();
		void connect_to_model(ApplicationModel * model);
	private:
		std::vector<AnalogOscView *> mAnalogOscs;
		std::vector<DigitalOscView *> mDigitalOscs;
		DelayView * mDelay;
		FilterView * mFilter;
		VCAView * mVCA;
		Env3View * mEnv3;
		FeedbackView * mFeedback;
		MiscAudioView * mMiscAudio;
};

#endif
