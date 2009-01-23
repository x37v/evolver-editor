#ifndef MISC_AUDIO_HPP
#define MISC_AUDIO_HPP

//this is for stuff that doesn't fit anywhere else
#include <QObject>

class MiscAudioModel : public QObject {
	Q_OBJECT
	public:
		MiscAudioModel(QObject * parent = NULL);
		enum distortion_type {
			internal,
			input
		};
		enum ext_in_mode_type {
			stereo,
			left,
			right,
			split
		};
		static const unsigned int volume_max;
		static const unsigned int distortion_max;
		static const unsigned int hack_max;
		static const unsigned int noise_volume_max;
		static const unsigned int ext_in_volume_max;
	private:
		unsigned int mVolume;
		distortion_type mDistortionType;
		unsigned int mDistortion;
		unsigned int mOutputHack;
		unsigned int mInputHack;
		unsigned int mNoiseVolume;
		ext_in_mode_type mExtInMode;
		unsigned int mExtInVolume;
};

#include <QWidget>
class SliderSpinBox;
class QGridLayout;
class QComboBox;

class MiscAudioView : public QWidget {
	Q_OBJECT
	public:
		MiscAudioView(QWidget * parent = NULL);
	private:
		QGridLayout * mLayout;

		SliderSpinBox * mVolume;

		QComboBox * mDistortionType;
		SliderSpinBox * mDistortion;

		SliderSpinBox * mOutputHack;
		SliderSpinBox * mInputHack;

		SliderSpinBox * mNoiseVolume;

		QComboBox * mExtInMode;
		SliderSpinBox * mExtInVolume;
};

#endif
