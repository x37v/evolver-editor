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

