#ifndef OSC_MODEL_HPP
#define OSC_MODEL_HPP

#include <QObject>

class OscModel : public QObject {
	Q_OBJECT
	public:
		OscModel(QObject * parent);
		virtual ~OscModel(){}
		static const unsigned int freq_max;
		static const unsigned int glide_max;
		static const unsigned int level_max;
		static const int tune_max;
		static const int tune_min;
	private:
		unsigned int mFreq;
		unsigned int mGlide;
		unsigned int mLevel;
		int mTune;
	signals:
		void freq_changed(unsigned int);
		void tune_changed(int);
		void glide_changed(unsigned int);
		void level_changed(unsigned int);
	public slots:
		void set_freq(unsigned int freq);
		void set_tune(int tune);
		void set_glide(unsigned int glide);
		void set_level(unsigned int lev);
};

class AnalogOscModel : public OscModel {
	Q_OBJECT
	public:
		AnalogOscModel(QObject * parent);
		enum shapes {
			saw,
			triangle,
			saw_triangle,
			pulse};
		static const unsigned int width_max;
	private:
		shapes mShape;
		unsigned int mWidth;
		//sync 2 -> 1
		bool mSyncing;
	signals:
		void shape_changed(shapes);
		void width_changed(unsigned int);
		void sync_changed(bool);
	public slots:
		void set_shape(shapes shape);
		void set_width(unsigned int width);
		//sync 2 -> 1
		void sync(bool s);
};

class DigitalOscModel : public OscModel {
	Q_OBJECT
	public:
		DigitalOscModel(QObject * parent);
		enum shape_mod {
			none,
			seq0,
			seq1,
			seq2,
			seq3
		};
		static const unsigned int shape_max;
		static const unsigned int fm_in_max;
		static const unsigned int ring_in_max;
	private:
		unsigned int mShape;
		unsigned int mFmIn;
		unsigned int mRingIn;
		shape_mod mShapeMod;
	public slots:
		void set_shape(unsigned int shape);
		void set_fm_in(unsigned int fm);
		void set_ring_in(unsigned int ring);
		void set_shape_mod(shape_mod mod);
	signals:
		void shape_changed(unsigned int shape);
		void fm_in_changed(unsigned int fm);
		void ring_in_changed(unsigned int ring);
		void shape_mod_changed(shape_mod mod);
};

#endif
