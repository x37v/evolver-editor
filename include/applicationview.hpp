#ifndef APPLICATION_VIEW_HPP
#define APPLICATION_VIEW_HPP

#include <QWidget>
#include <vector>

class LFOView;
class AnalogOscView;
class DigitalOscView;
class ModRoutingView;
class DelayView;
class FilterView;
class VCAView;
class Env3View;
class FeedbackView;
class MiscAudioView;

class ApplicationView : public QWidget {
	Q_OBJECT
	public:
		ApplicationView(QWidget * parent = NULL);
	private:
		std::vector<AnalogOscView *> mAnalogOscs;
		std::vector<DigitalOscView *> mDigitalOscs;
		std::vector<LFOView *> mLFOs;
		std::vector<ModRoutingView *> mMods;
		DelayView * mDelay;
		FilterView * mFilter;
		VCAView * mVCA;
		Env3View * mEnv3;
		FeedbackView * mFeedback;
		MiscAudioView * mMiscAudio;
};

#endif

