#ifndef FILTER_HPP
#define FILTER_HPP

#include <QObject>

class FilterModel : public QObject {
	Q_OBJECT
	public:
		FilterModel(QObject * parent = NULL);
		enum lpf_mode {
			two_pole,
			four_pole
		};
		enum hpf_mode {
			after_lpf,
			external_input
		};
		static const unsigned int cut_freq_max;
		static const unsigned int resonance_max;
		static const unsigned int key_amount_max;
		static const unsigned int audio_mod_max;
		static const unsigned int split_max;
		static const int env_amount_min;
		static const int env_amount_max;
		static const unsigned int env_attack_max;
		static const unsigned int env_decay_max;
		static const unsigned int env_sustain_max;
		static const unsigned int env_release_max;
		static const unsigned int env_velocity_max;
		static const unsigned int hpf_freq_max;
	public slots:
		void set_mode(int val);
		void set_cutoff(int val);
		void set_resonance(int val);
		void set_key_amount(int val);
		void set_audio_mod(int val);
		void set_split(int val);
		void set_env_amount(int val);
		void set_env_attack(int val);
		void set_env_decay(int val);
		void set_env_sustain(int val);
		void set_env_release(int val);
		void set_env_velocity(int val);
		void set_hpf_mode(int val);
		void set_hpf_cutoff(int val);
	signals:
		void mode_changed(int val);
		void cutoff_changed(int val);
		void resonance_changed(int val);
		void key_amount_changed(int val);
		void audio_mod_changed(int val);
		void split_changed(int val);
		void env_amount_changed(int val);
		void env_attack_changed(int val);
		void env_decay_changed(int val);
		void env_sustain_changed(int val);
		void env_release_changed(int val);
		void env_velocity_changed(int val);
		void hpf_mode_changed(int val);
		void hpf_cutoff_changed(int val);
	private:
		lpf_mode mMode;
		unsigned int mCutFreq;
		unsigned int mResonance;
		unsigned int mKeyAmount;
		unsigned int mAudioMod;
		unsigned int mSplit;

		//envlope;
		int mEnvAmt;
		unsigned int mEnvAttack;
		unsigned int mEnvDecay;
		unsigned int mEnvSustain;
		unsigned int mEnvRelease;
		unsigned int mEnvVelocity;

		//hpf
		hpf_mode mHPFMode;
		unsigned int mHPFFreq;
};

#include <QWidget>

class QComboBox;
class SliderSpinBox;
class QGridLayout;

class FilterView : public QWidget {
	Q_OBJECT
	public:
		FilterView(QWidget * parent = NULL);
		void connect_to_model(FilterModel * model);
	public slots:
		void set_mode(int val);
		void set_cutoff(int val);
		void set_resonance(int val);
		void set_key_amount(int val);
		void set_audio_mod(int val);
		void set_split(int val);
		void set_env_amount(int val);
		void set_env_attack(int val);
		void set_env_decay(int val);
		void set_env_sustain(int val);
		void set_env_release(int val);
		void set_env_velocity(int val);
		void set_hpf_mode(int val);
		void set_hpf_cutoff(int val);
	signals:
		void mode_changed(int val);
		void cutoff_changed(int val);
		void resonance_changed(int val);
		void key_amount_changed(int val);
		void audio_mod_changed(int val);
		void split_changed(int val);
		void env_amount_changed(int val);
		void env_attack_changed(int val);
		void env_decay_changed(int val);
		void env_sustain_changed(int val);
		void env_release_changed(int val);
		void env_velocity_changed(int val);
		void hpf_mode_changed(int val);
		void hpf_cutoff_changed(int val);
	private:
		QGridLayout * mLayout;

		QComboBox * mMode;
		SliderSpinBox * mCutFreq;
		SliderSpinBox * mResonance;
		SliderSpinBox * mKeyAmount;
		SliderSpinBox * mAudioMod;
		SliderSpinBox * mSplit;

		SliderSpinBox * mEnvAmt;
		SliderSpinBox * mEnvAttack;
		SliderSpinBox * mEnvDecay;
		SliderSpinBox * mEnvSustain;
		SliderSpinBox * mEnvRelease;
		SliderSpinBox * mEnvVelocity;

		//hpf
		QComboBox * mHPFMode;
		SliderSpinBox * mHPFFreq;
};

#endif
