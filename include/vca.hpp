#ifndef VCA_HPP
#define VCA_HPP

#include <QObject>
class VCAModel : public QObject {
	Q_OBJECT
	public:
		VCAModel(QObject * parent = NULL);
		static const unsigned int level_max;
		static const unsigned int env_amount_max;
		static const unsigned int attack_max;
		static const unsigned int decay_max;
		static const unsigned int sustain_max;
		static const unsigned int release_max;
		static const unsigned int velocity_max;
		static const char * pan_type_names[];
		static const unsigned int num_pan_types;
		enum pan_type {
			stereo_one,
			stereo_two,
			stereo_three,
			mono,
			rev_stereo_one,
			rev_stereo_two,
			rev_stereo_three
		};
	public slots:
		void set_level(int val);
		void set_env_amount(int val);
		void set_attack(int val);
		void set_decay(int val);
		void set_sustain(int val);
		void set_release(int val);
		void set_velocity(int val);
		void set_pan(int val);
	signals:
		void level_changed(int val);
		void env_amount_changed(int val);
		void attack_changed(int val);
		void decay_changed(int val);
		void sustain_changed(int val);
		void release_changed(int val);
		void velocity_changed(int val);
		void pan_changed(int val);
	private:
		unsigned int mLevel;
		unsigned int mEnvAmount;
		unsigned int mAttack;
		unsigned int mDecay;
		unsigned int mSustain;
		unsigned int mRelease;
		unsigned int mVelocity;
		pan_type mPanType;
};

#include <QWidget>
class QComboBox;
class SliderSpinBox;
class QGridLayout;

class VCAView : public QWidget {
	Q_OBJECT
	public:
		VCAView(QWidget * parent = NULL);
		void connect_to_model(VCAModel * model);
	private:
		QGridLayout * mLayout;

		SliderSpinBox * mLevel;
		SliderSpinBox * mEnvAmount;
		SliderSpinBox * mAttack;
		SliderSpinBox * mDecay;
		SliderSpinBox * mSustain;
		SliderSpinBox * mRelease;
		SliderSpinBox * mVelocity;
		QComboBox * mPanType;
	public slots:
		void set_level(int val);
		void set_env_amount(int val);
		void set_attack(int val);
		void set_decay(int val);
		void set_sustain(int val);
		void set_release(int val);
		void set_velocity(int val);
		void set_pan(int val);
	signals:
		void level_changed(int val);
		void env_amount_changed(int val);
		void attack_changed(int val);
		void decay_changed(int val);
		void sustain_changed(int val);
		void release_changed(int val);
		void velocity_changed(int val);
		void pan_changed(int val);
};

#endif
