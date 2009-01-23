#ifndef DELAY_HPP
#define DELAY_HPP

#include <QObject>

class DelayModel : public QObject {
	Q_OBJECT
	public:
		DelayModel(QObject * parent = NULL);
		const static unsigned int delay_time_max;
		const static unsigned int delay_level_max;
		const static unsigned int feedback_level_max;
		const static unsigned int num_delay_types;
		const static char* delay_type_names[];
		enum sync_type {
			off,
			steps_32,
			steps_16,
			steps_8,
			steps_4,
			steps_2,
			step,
			step_div_2,
			step_div_4,
			step_div_8,
			step_div_16,
			steps_6,
			steps_3,
			one_and_half_steps,
			step_half,
			step_div_3,
			step_div_6
		};
	public slots:
		void set_delay_time(unsigned int index, int time);
		void set_delay_level(unsigned int index, int level);
		void set_delay_sync(unsigned int index, sync_type t);
		void set_feedback_level(int level);
		void set_filter_feedback_level(int level);
	signals:
		void delay_time_changed(unsigned int index, int time);
		void delay_level_changed(unsigned int index, int time);
		void delay_sync_changed(unsigned int index, int time);
		void feedback_level_changed(int);
		void filter_feedback_level_changed(int);
	private:
		unsigned int mDelayTime[3];
		unsigned int mDelayLevel[3];
		sync_type mDelaySync[3];
		unsigned int mFeedbackLevel;
		unsigned int mFilterFeedbackLevel;
};

#include <QWidget>
class SliderSpinBox;
class QComboBox;
class QGridLayout;

class DelayView : public QWidget {
	Q_OBJECT
	public:
		DelayView(QWidget * parent = NULL);
	private:
		SliderSpinBox * mDelayTime[3];
		SliderSpinBox * mDelayLevel[3];
		QComboBox * mDelaySync[3];
		SliderSpinBox * mFeedbackLevel;
		SliderSpinBox * mFilterFeedbackLevel;
		QGridLayout * mLayout;
};

#endif
