#ifndef FEEDBACK_HPP
#define FEEDBACK_HPP

#include <QObject>

class FeedbackModel : public QObject {
	Q_OBJECT
	public:
		FeedbackModel(QObject * parent = NULL);
		static const unsigned int freq_max;
		static const unsigned int level_max;
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
	private:
		QGridLayout * mLayout;

		SliderSpinBox * mFreq;
		SliderSpinBox * mLevel;
		QPushButton * mGrunge;
};

#endif
