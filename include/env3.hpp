#ifndef ENV3_HPP
#define ENV3_HPP

#include <QObject>
class Env3Model : public QObject {
	Q_OBJECT
	public:
		Env3Model(QObject * parent = NULL);
		static const unsigned int delay_max;
		static const int amount_min;
		static const int amount_max;
		static const unsigned int attack_max;
		static const unsigned int decay_max;
		static const unsigned int sustain_max;
		static const unsigned int release_max;
		static const unsigned int velocity_max;
	private:
		unsigned int mDestination;
		unsigned int mDelay;
		int mAmount;
		unsigned int mAttack;
		unsigned int mDecay;
		unsigned int mSustain;
		unsigned int mRelease;
		unsigned int mVelocity;
};

#include <QWidget>
class QComboBox;
class SliderSpinBox;
class QGridLayout;

class Env3View : public QWidget {
	Q_OBJECT
	public:
		Env3View(QWidget * parent = NULL);
	private:
		QGridLayout * mLayout;

		QComboBox * mDestSelect;

		SliderSpinBox * mDelay;
		SliderSpinBox * mAmount;
		SliderSpinBox * mAttack;
		SliderSpinBox * mDecay;
		SliderSpinBox * mSustain;
		SliderSpinBox * mRelease;
		SliderSpinBox * mVelocity;
};

#endif
