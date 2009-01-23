#include "delay.hpp"

const unsigned int DelayModel::delay_time_max = 150;
const unsigned int DelayModel::delay_level_max = 100;
const unsigned int DelayModel::feedback_level_max = 100;
const unsigned int DelayModel::num_delay_types = 17;
const char* DelayModel::delay_type_names[]  = {
	"none",
	"32 steps",
	"16 steps",
	"8 steps",
	"4 steps",
	"2 steps",
	"1 step",
	"1/2 step",
	"1/4 step",
	"1/8 step",
	"1/16 step",
	"6 steps",
	"3 steps",
	"3/2 steps",
	"1/2 step",
	"1/3 step",
	"1/6 step"
};

DelayModel::DelayModel(QObject * parent) : QObject(parent) {
	for(unsigned int i = 0; i < 3; i++){
		mDelaySync[i] = off;
		mDelayLevel[i] = 0;
		mDelayTime[i] = 0;
	}
	mFeedbackLevel = 0;
	mFilterFeedbackLevel = 0;
}

void DelayModel::set_delay_time(unsigned int index, int time){
	if (index > 2)
		return;
	if (time >= 0 && 
			(unsigned int)time <= delay_time_max &&
			(unsigned int)time != mDelayTime[index]) {
		mDelayTime[index] = time;
		emit(delay_time_changed(index, mDelayTime[index]));
	}
}

void DelayModel::set_delay_level(unsigned int index, int level){
	if (index > 2)
		return;
	if(level >= 0 &&
			(unsigned int)level <= delay_level_max &&
			(unsigned int)level != mDelayLevel[index]){
		mDelayLevel[index] = level;
		emit(delay_level_changed(index, mDelayLevel[index]));
	}
}

void DelayModel::set_delay_sync(unsigned int index, sync_type t){
	if (index > 2)
		return;
	if(t != mDelaySync[index]){
		mDelaySync[index] = t;
		emit(delay_sync_changed(index, mDelaySync[index]));
	}
}

void DelayModel::set_feedback_level(int level){
	if (level >= 0 && 
			(unsigned int)level <= feedback_level_max && 
			(unsigned int)level != mFeedbackLevel){
		mFeedbackLevel = level;
		emit(feedback_level_changed(mFeedbackLevel));
	}
}

void DelayModel::set_filter_feedback_level(int level){
	if (level >= 0 && 
			(unsigned int)level <= feedback_level_max && 
			(unsigned int)level != mFilterFeedbackLevel){
		mFilterFeedbackLevel = level;
		emit(filter_feedback_level_changed(mFilterFeedbackLevel));
	}
}

#include <QComboBox>
#include <QLabel>
#include <QGridLayout>
#include "sliderspinbox.hpp"

DelayView::DelayView(QWidget * parent) : QWidget(parent) {
	QLabel * lab;
	mLayout = new QGridLayout(this);

	mFeedbackLevel = new SliderSpinBox(this);
	mFilterFeedbackLevel = new SliderSpinBox(this);
	mFeedbackLevel->setRange(0, DelayModel::feedback_level_max);
	mFilterFeedbackLevel->setRange(0, DelayModel::feedback_level_max);

	for(unsigned int i = 0; i < 3; i++){
		mDelaySync[i] = new QComboBox(this);
		mDelayTime[i] = new SliderSpinBox(this);
		mDelayLevel[i] = new SliderSpinBox(this);

		lab = new QLabel(QString("%1 level").arg(i + 1), this);
		mLayout->addWidget(lab, 0 + i * 3, 0, Qt::AlignRight);
		mLayout->addWidget(mDelayLevel[i], 0 + i * 3, 1);

		lab = new QLabel(QString("%1 sync").arg(i + 1), this);
		mLayout->addWidget(lab, 1 + i * 3, 0, Qt::AlignRight);
		mLayout->addWidget(mDelaySync[i], 1 + i * 3, 1);

		lab = new QLabel(QString("%1 time").arg(i + 1), this);
		mLayout->addWidget(lab, 2 + i * 3, 0, Qt::AlignRight);
		mLayout->addWidget(mDelayTime[i], 2 + i * 3, 1);

		mDelayTime[i]->setRange(0, DelayModel::delay_time_max);
		mDelayLevel[i]->setRange(0, DelayModel::delay_level_max);
		for(unsigned int j = 0; j < DelayModel::num_delay_types; j++)
			mDelaySync[i]->addItem(QString(DelayModel::delay_type_names[j]));
	}
	lab = new QLabel(QString("fbdk"), this);
	mLayout->addWidget(lab, 9, 0, Qt::AlignRight);
	mLayout->addWidget(mFeedbackLevel, 9, 1);

	lab = new QLabel(QString("fbdk to filter"), this);
	mLayout->addWidget(lab, 10, 0, Qt::AlignRight);
	mLayout->addWidget(mFilterFeedbackLevel, 10, 1);

	mLayout->setSpacing(1);
	setLayout(mLayout);
}

