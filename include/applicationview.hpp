#ifndef APPLICATION_VIEW_HPP
#define APPLICATION_VIEW_HPP

#include <QWidget>
#include <vector>

class LFOView;
class AnalogOscView;
class DigitalOscView;
class ModView;
class DelayView;
class FilterView;

class ApplicationView : public QWidget {
	Q_OBJECT
	public:
		ApplicationView(QWidget * parent = NULL);
	private:
		std::vector<AnalogOscView *> mAnalogOscs;
		std::vector<DigitalOscView *> mDigitalOscs;
		std::vector<LFOView *> mLFOs;
		std::vector<ModView *> mMods;
		DelayView * mDelay;
		FilterView * mFilter;
};

#endif

