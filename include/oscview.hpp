#ifndef OSC_VIEW_HPP
#define OSC_VIEW_HPP

#include <QWidget>
#include <vector>

class SliderSpinBox;
class QGridLayout;
class QRadioButton;
class QComboBox;
class QLabel;

class OscView : public QWidget {
	Q_OBJECT
	public:
		OscView(QWidget * parent = NULL);
		std::vector<QLabel *> * labels();
	protected:
		SliderSpinBox * mFreqSlider;
		SliderSpinBox * mTuneSlider;
		SliderSpinBox * mGlideSlider;
		SliderSpinBox * mLevelSlider;
		QGridLayout * mLayout;
		std::vector<QLabel *> mLables;
	signals:
		void freq_changed(int);
		void tune_changed(int);
		void glide_changed(int);
		void level_changed(int);
	public slots:
		void set_freq(int freq);
		void set_tune(int tune);
		void set_glide(int glide);
		void set_level(int lev);
		void show_labels(bool show);
};

class AnalogOscView : public OscView {
	Q_OBJECT
	private:
		SliderSpinBox * mWidthSlider;
		QComboBox * mShapeSelect;
	public:
		AnalogOscView(QWidget * parent = NULL);
};

class DigitalOscView : public OscView {
	Q_OBJECT
	private:
		SliderSpinBox * mShapeSlider;
		SliderSpinBox * mFmInSlider;
		SliderSpinBox * mRingInSlider;
		QComboBox * mShapeSeqSelect;
	public:
		DigitalOscView(QWidget * parent = NULL);
};

#endif
