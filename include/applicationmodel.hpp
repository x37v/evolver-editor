#ifndef APPLICATION_MODEL_HPP
#define APPLICATION_MODEL_HPP

#include <QObject>
#include <vector>
#include "model.hpp"

class AnalogOscModel;
class DigitalOscModel;
class LFOModel;
class ModRoutingModel;

class FilterModel;
class FeedbackModel;
class VCAModel;
class Env3Model;
class DelayModel;
class FeedbackModel;
class MiscAudioModel;
class MiscModulationModel;

class ApplicationModel : public Model {
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
		FeedbackModel * feedback();
		MiscAudioModel * misc_audio();
		MiscModulationModel * misc_modulations();
	private slots:
		void osc_set_freq(int index);
		void osc_set_tune(int index);
		void osc_set_glide(int index);
		void osc_set_level(int index);
		void osc_set_glide_mode(int index);
		void analog_osc_set_shape(int index);
		void analog_osc_set_width(int index);
		void analog_osc_set_sync(int index);
		void digital_osc_set_shape(int index);
		void digital_osc_set_fm_in(int index);
		void digital_osc_set_ring_in(int index);
		void digital_osc_set_shape_mod(int index);

		void lfo_set_freq(int index);
		void lfo_set_amount(int index);
		void lfo_set_shape(int index);
		//uses amount
		//void lfo_set_key_sync(int index);
		//uses freq
		//void lfo_set_sync_type(int index);
		void lfo_set_destination(int index);

		void mod_set_amount(int index);
		void mod_set_source(int index);
		void mod_set_destination(int index);
	private:
		std::vector<AnalogOscModel *> mAnalogOscs;
		std::vector<DigitalOscModel *> mDigitalOscs;
		std::vector<LFOModel *> mLFOs;
		std::vector<ModRoutingModel *> mMods;
		FilterModel * mFilter;
		VCAModel * mVCA;
		Env3Model * mEnv3;
		DelayModel * mDelay;
		FeedbackModel * mFeedback;
		MiscAudioModel * mMiscAudio;
		MiscModulationModel * mMiscModulation;
};

#endif
