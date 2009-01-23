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
		void set_shape(int s);
		void set_key_sync(bool sync);
		void set_sync_type(int t);
		void set_destination(int dest);
	signals:
		void freq_changed(int freq);
		void amount_changed(int amount);
		void shape_changed(int shape);
		void key_sync_changed(bool sync);
		void sync_type_changed(int t);
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
class ModDestComboBox;

class LFOView : public QWidget {
	Q_OBJECT
	public:
		LFOView(QWidget * parent = NULL);
		std::vector<QLabel *> * labels();
		void connect_to_model(LFOModel * model);

	private:
		ModDestComboBox * mDestSelect;
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
		void set_freq(int freq);
		void set_amount(int amount);
		void set_shape(int s);
		void set_key_sync(bool sync);
		void set_sync_type(int t);
		void set_destination(int dest);
	signals:
		void freq_changed(int freq);
		void amount_changed(int amount);
		void shape_changed(int shape);
		void key_sync_changed(bool sync);
		void sync_type_changed(int t);
		void destination_changed(int dest);
};

#endif
