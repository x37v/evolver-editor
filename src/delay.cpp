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

DelayModel::DelayModel(QObject * parent) : Model(parent) {
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
		if(mDelaySync[index] == off){
			switch(index){
				case 0:
					send_program_param(35, mDelayTime[index]);
					break;
				case 1:
					send_program_param(99, mDelayTime[index]);
					break;
				case 2:
					send_program_param(101, mDelayTime[index]);
					break;
				default:
					break;
			}
		}
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
		switch(index){
			case 0:
				send_program_param(36, mDelayLevel[index]);
				break;
			case 1:
				send_program_param(100, mDelayLevel[index]);
				break;
			case 2:
				send_program_param(102, mDelayLevel[index]);
				break;
			default:
				break;
		}
	}
}

void DelayModel::set_delay_sync(unsigned int index, int t){
	sync_type type = (sync_type)t;
	if (index > 2)
		return;
	if(type != mDelaySync[index]){
		mDelaySync[index] = type;
		emit(delay_sync_changed(index, mDelaySync[index]));
		switch(index){
			case 0:
				if(mDelaySync[index] == off)
					send_program_param(35, mDelayTime[index]);
				else
					send_program_param(35, mDelaySync[index] + 151);
				break;
			case 1:
				if(mDelaySync[index] == off)
					send_program_param(99, mDelayTime[index]);
				else
					send_program_param(99, mDelaySync[index] + 151);
				break;
			case 2:
				if(mDelaySync[index] == off)
					send_program_param(101, mDelayTime[index]);
				else
					send_program_param(101, mDelaySync[index] + 151);
				break;
			default:
				break;
		}
	}
}

void DelayModel::set_feedback_level(int level){
	if (level >= 0 && 
			(unsigned int)level <= feedback_level_max && 
			(unsigned int)level != mFeedbackLevel){
		mFeedbackLevel = level;
		emit(feedback_level_changed(mFeedbackLevel));
		send_program_param(37, mFeedbackLevel);
	}
}

void DelayModel::set_filter_feedback_level(int level){
	if (level >= 0 && 
			(unsigned int)level <= feedback_level_max && 
			(unsigned int)level != mFilterFeedbackLevel){
		mFilterFeedbackLevel = level;
		emit(filter_feedback_level_changed(mFilterFeedbackLevel));
		send_program_param(38, mFilterFeedbackLevel);
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

	//connect internal signals/slots
	QObject::connect(mDelayTime[0], SIGNAL(valueChanged(int)),
			this, SLOT(set_delay_time_0(int)));
	QObject::connect(mDelayLevel[0], SIGNAL(valueChanged(int)),
			this, SLOT(set_delay_level_0(int)));
	QObject::connect(mDelaySync[0], SIGNAL(currentIndexChanged(int)),
			this, SLOT(set_delay_sync_0(int)));

	QObject::connect(mDelayTime[1], SIGNAL(valueChanged(int)),
			this, SLOT(set_delay_time_1(int)));
	QObject::connect(mDelayLevel[1], SIGNAL(valueChanged(int)),
			this, SLOT(set_delay_level_1(int)));
	QObject::connect(mDelaySync[1], SIGNAL(currentIndexChanged(int)),
			this, SLOT(set_delay_sync_1(int)));

	QObject::connect(mDelayTime[2], SIGNAL(valueChanged(int)),
			this, SLOT(set_delay_time_2(int)));
	QObject::connect(mDelayLevel[2], SIGNAL(valueChanged(int)),
			this, SLOT(set_delay_level_2(int)));
	QObject::connect(mDelaySync[2], SIGNAL(currentIndexChanged(int)),
			this, SLOT(set_delay_sync_2(int)));

	//connect out signals
	QObject::connect(mFeedbackLevel,
			SIGNAL(valueChanged(int)),
			this, 
			SIGNAL(feedback_level_changed(int)));
	QObject::connect(mFilterFeedbackLevel,
			SIGNAL(valueChanged(int)),
			this, 
			SIGNAL(filter_feedback_level_changed(int)));
}

void DelayView::connect_to_model(DelayModel * model){

	QObject::connect(
			this,
			SIGNAL(delay_time_changed(unsigned int, int)),
			model,
			SLOT(set_delay_time(unsigned int, int)));
	QObject::connect(
			model,
			SIGNAL(delay_time_changed(unsigned int, int)),
			this,
			SLOT(set_delay_time(unsigned int, int)));

	QObject::connect(
			this,
			SIGNAL(delay_level_changed(unsigned int, int)),
			model,
			SLOT(set_delay_level(unsigned int, int)));
	QObject::connect(
			model,
			SIGNAL(delay_level_changed(unsigned int, int)),
			this,
			SLOT(set_delay_level(unsigned int, int)));

	QObject::connect(
			this,
			SIGNAL(delay_sync_changed(unsigned int, int)),
			model,
			SLOT(set_delay_sync(unsigned int, int)));
	QObject::connect(
			model,
			SIGNAL(delay_sync_changed(unsigned int, int)),
			this,
			SLOT(set_delay_sync(unsigned int, int)));

	QObject::connect(
			this,
			SIGNAL(feedback_level_changed(int)),
			model,
			SLOT(set_feedback_level(int)));
	QObject::connect(
			model,
			SIGNAL(feedback_level_changed(int)),
			this,
			SLOT(set_feedback_level(int)));

	QObject::connect(
			this,
			SIGNAL(filter_feedback_level_changed(int)),
			model,
			SLOT(set_filter_feedback_level(int)));
	QObject::connect(
			model,
			SIGNAL(filter_feedback_level_changed(int)),
			this,
			SLOT(set_filter_feedback_level(int)));
}

void DelayView::set_delay_time_0(int time){
	emit(delay_time_changed(0, time));
}

void DelayView::set_delay_level_0(int level){
	emit(delay_level_changed(0, level));
}

void DelayView::set_delay_sync_0(int s){
	emit(delay_sync_changed(0, s));
}

void DelayView::set_delay_time_1(int time){
	emit(delay_time_changed(1, time));
}

void DelayView::set_delay_level_1(int level){
	emit(delay_level_changed(1, level));
}

void DelayView::set_delay_sync_1(int s){
	emit(delay_sync_changed(1, s));
}

void DelayView::set_delay_time_2(int time){
	emit(delay_time_changed(2, time));
}

void DelayView::set_delay_level_2(int level){
	emit(delay_level_changed(2, level));
}

void DelayView::set_delay_sync_2(int s){
	emit(delay_sync_changed(2, s));
}

void DelayView::set_delay_time(unsigned int index, int time){
	if(index > 2)
		return;
	mDelayTime[index]->setValue(time);
}

void DelayView::set_delay_level(unsigned int index, int level){
	if(index > 2)
		return;
	mDelayLevel[index]->setValue(level);
}

void DelayView::set_delay_sync(unsigned int index, int t){
	if(index > 2)
		return;
	mDelaySync[index]->setCurrentIndex(t);
}

void DelayView::set_feedback_level(int level){
	mFeedbackLevel->setValue(level);
}

void DelayView::set_filter_feedback_level(int level){
	mFilterFeedbackLevel->setValue(level);
}


