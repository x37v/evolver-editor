#ifndef APPLICATION_MODEL_HPP
#define APPLICATION_MODEL_HPP

#include <QObject>
#include <vector>

class AnalogOscModel;
class DigitalOscModel;
class LFOModel;
class ModRoutingModel;

class FilterModel;
class VCAModel;
class Env3Model;
class DelayModel;
class FeedbackModel;
class MiscAudioModel;
class MiscModulationModel;

class ApplicationModel : public QObject {
	Q_OBJECT
	public:
		ApplicationModel(QObject * parent = NULL);

		std::vector<AnalogOscModel *> * analog_oscs();
		std::vector<DigitalOscModel *> * digital_oscs();
		std::vector<LFOModel *> * lfos();
		std::vector<ModRoutingModel *> * mods();
		FilterModel * filter();
		VCAModel * vca();
		Env3Model * env3();
		DelayModel * delay();
		FilterModel * feedback();
		MiscAudioModel * misc_audio();
		MiscModulationModel * misc_modulations();
	private:
		std::vector<AnalogOscModel *> mAnalogOscs;
		std::vector<DigitalOscModel *> mDigitalOscs;
		std::vector<LFOModel *> mLFOs;
		std::vector<ModRoutingModel *> mMods;
		FilterModel * mFilter;
		VCAModel * mVCA;
		Env3Model * mEnv3;
		DelayModel * mDelay;
		FilterModel * mFeedback;
		MiscAudioModel * mMiscAudio;
		MiscModulationModel * mMiscModulation;
};

#endif
