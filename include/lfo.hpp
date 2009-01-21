#ifndef LFO_HPP
#define LFO_HPP

#include <QObject>

class LFOModel : public QObject {
	Q_OBJECT
	public:
		LFOModel(QObject * parent = NULL);
		static const unsigned int freq_max;
		static const unsigned int amount_max;
		enum shapes {
			triangle,
			rev_saw,
			saw,
			pulse,
			random
		};
		enum sync_types {
			off,
			seq_div_32,
			seq_div_16,
			seq_div_8,
			seq_div_4,
			seq_div_2,
			one_per_step,
			two_per_step,
			four_per_step,
			eight_per_step,
			sixteen_per_step
		};
	public slots:
		void set_freq(int freq);
		void set_amount(int amount);
		void set_shape(shapes shape);
		void set_key_sync(bool sync);
		void set_sync_type(sync_types t);
		void set_destination(int dest);
	signals:
		void freq_changed(int freq);
		void amount_changed(int amount);
		void shape_changed(shapes shape);
		void key_sync_changed(bool sync);
		void sync_type_changed(sync_types t);
		void destination_changed(int dest);
	private:
		unsigned int mAmount;
		unsigned int mFreq;
		shapes mShape;
		bool mKeySync;
		sync_types mSyncType;
		unsigned int mDestination;
};

#include <QWidget>
#include <vector>
class QComboBox;
class SliderSpinBox;
class QPushButton;
class QGridLayout;
class QLabel;

class LFOView : public QWidget {
	Q_OBJECT
	public:
		LFOView(QWidget * parent = NULL);
		std::vector<QLabel *> * labels();

	private:
		QComboBox * mDestSelect;
		QComboBox * mSyncSelect;
		QComboBox * mShapeSelect;
		SliderSpinBox * mFreqSlider;
		SliderSpinBox * mAmountSlider;
		QPushButton * mKeySyncButton;
		QGridLayout * mLayout;
	protected:
		std::vector<QLabel *> mLabels;
	public slots:
		void show_labels(bool show);
	signals:
		void shape_changed(int);
		void sync_changed(int);
		void modulation_dest_changed(int);
		void freq_changed(int);
		void amount_changed(int);
		void key_sync_changed(bool);
};

#include <stdexcept>
class QVBoxLayout;

class LFOArrayView : public QWidget {
	Q_OBJECT
	public:
		LFOArrayView(QWidget * parent = NULL);
		LFOView * operator[] (const int index);
	private:
		std::vector<LFOView *> mLFOs;
		QVBoxLayout * mLayout;
};

#endif
