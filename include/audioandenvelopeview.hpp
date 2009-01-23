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

//this is a view/window which include the audio an envelope control settings

class AudioAndEnvelopeView : public QWidget {
	Q_OBJECT
	public:
		AudioAndEnvelopeView(QWidget * parent = NULL);
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
