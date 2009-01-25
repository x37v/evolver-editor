#ifndef OSC_VIEW_HPP
#define OSC_VIEW_HPP

#include <QWidget>
#include <vector>

class SliderSpinBox;
class QGridLayout;
class QPushButton;
class QComboBox;
class QLabel;

class OscModel;
class AnalogOscModel;
class DigitalOscModel;

class OscView : public QWidget {
	Q_OBJECT
	public:
		OscView(QWidget * parent = NULL);
		void connect_to_model(OscModel * model);
		std::vector<QLabel *> * labels();
	protected:
		SliderSpinBox * mFreqSlider;
		SliderSpinBox * mTuneSlider;
		QComboBox * mGlideMode;
		SliderSpinBox * mGlideSlider;
		SliderSpinBox * mLevelSlider;
		QGridLayout * mLayout;
		std::vector<QLabel *> mLables;
	signals:
		void freq_changed(int);
		void tune_changed(int);
		void glide_changed(int);
		void glide_mode_changed(int);
		void level_changed(int);
	public slots:
		void set_freq(int freq);
		void set_tune(int tune);
		void set_glide(int glide);
		void set_glide_mode(int m);
		void set_level(int lev);
		void show_labels(bool show);
};

class AnalogOscView : public OscView {
	Q_OBJECT
	private:
		SliderSpinBox * mWidthSlider;
		QComboBox * mShapeSelect;
		QPushButton * mSyncButton;
	public:
		AnalogOscView(QWidget * parent = NULL);
		void connect_to_model(AnalogOscModel * model);
	signals:
		void shape_changed(int);
		void width_changed(int);
		void sync_changed(bool);
	public slots:
		void show_sync_button(bool show);
		void set_shape(int shape);
		void set_width(int width);
		//sync 2 -> 1
		void set_sync(bool s);
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
		void connect_to_model(DigitalOscModel * model);
	public slots:
		void set_shape(int shape);
		void set_fm_in(int fm);
		void set_ring_in(int ring);
		void set_shape_mod(int mod);
	signals:
		void shape_changed(int shape);
		void fm_in_changed(int fm);
		void ring_in_changed(int ring);
		void shape_mod_changed(int mod);
};

#endif
