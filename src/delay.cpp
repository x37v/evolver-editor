#include "delay.hpp"

const unsigned int DelayModel::delay_time_max = 150;
const unsigned int DelayModel::delay_level_max = 100;
const unsigned int DelayModel::feedback_level_max = 100;

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
