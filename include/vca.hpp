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
};

#endif
