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
		enum glide_mode_t {
			normal,
			fingered,
			off
		};
		unsigned int freq();
		unsigned int glide();
		glide_mode_t glide_mode();
		unsigned int level();
		int tune();
	private:
		unsigned int mFreq;
		unsigned int mGlide;
		unsigned int mLevel;
		int mTune;
		glide_mode_t mGlideMode;
	signals:
		void freq_changed(int);
		void tune_changed(int);
		void glide_changed(int);
		void level_changed(int);
		void glide_mode_changed(int);
	public slots:
		void set_freq(int freq);
		void set_tune(int tune);
		void set_glide(int glide);
		void set_level(int lev);
		void set_glide_mode(int m);
};

class AnalogOscModel : public OscModel {
	Q_OBJECT
	public:
		AnalogOscModel(QObject * parent);
		enum shape_t {
			saw,
			triangle,
			saw_triangle,
			pulse};
		static const unsigned int width_max;
		shape_t shape();
		unsigned int width();
		bool sync();
	private:
		shape_t mShape;
		unsigned int mWidth;
		//sync 2 -> 1
		bool mSyncing;
	signals:
		void shape_changed(int);
		void width_changed(int);
		void sync_changed(bool);
	public slots:
		void set_shape(int shape);
		void set_width(int width);
		//sync 2 -> 1
		void set_sync(bool s);
};

class DigitalOscModel : public OscModel {
	Q_OBJECT
	public:
		DigitalOscModel(QObject * parent);
		enum shape_mod_t {
			none,
			seq0,
			seq1,
			seq2,
			seq3
		};
		static const unsigned int shape_max;
		static const unsigned int fm_in_max;
		static const unsigned int ring_in_max;
		unsigned int shape();
		unsigned int fm_in();
		unsigned int ring_in();
		shape_mod_t shape_mod();
	private:
		unsigned int mShape;
		unsigned int mFmIn;
		unsigned int mRingIn;
		shape_mod_t mShapeMod;
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
