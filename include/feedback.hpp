#ifndef FEEDBACK_HPP
#define FEEDBACK_HPP

#include <QObject>

class FeedbackModel : public QObject {
	Q_OBJECT
	public:
		FeedbackModel(QObject * parent = NULL);
		static const unsigned int freq_max;
		static const unsigned int level_max;
	public slots:
		void set_freq(int f);
		void set_level(int l);
		void set_grunge(bool g);
	signals:
		void freq_changed(int);
		void level_changed(int);
		void grunge_changed(bool);
	private:
		unsigned int mFreq;
		unsigned int mLevel;
		bool mGrunge;
};

#include <QWidget>
class QPushButton;
class QGridLayout;
class SliderSpinBox;

class FeedbackView : public QWidget {
	Q_OBJECT
	public:
		FeedbackView(QWidget * parent = NULL);
		void connect_to_model(FeedbackModel * model);
	public slots:
		void set_freq(int f);
		void set_level(int l);
		void set_grunge(bool g);
	signals:
		void freq_changed(int);
		void level_changed(int);
		void grunge_changed(bool);
	private:
		QGridLayout * mLayout;

		SliderSpinBox * mFreq;
		SliderSpinBox * mLevel;
		QPushButton * mGrunge;
};

#endif
