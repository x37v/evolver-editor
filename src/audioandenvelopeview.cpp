#include "audioandenvelopeview.hpp"
#include "oscview.hpp"
#include "delay.hpp"
#include "filter.hpp"
#include "vca.hpp"
#include "env3.hpp"
#include "feedback.hpp"
#include "miscaudio.hpp"
#include "lfo.hpp"
#include <QSplitter>
#include <QVBoxLayout>
#include <QLabel>
#include "titledwidget.hpp"
#include "applicationmodel.hpp"

AudioAndEnvelopeView::AudioAndEnvelopeView(QWidget * parent) : QWidget(parent){
	QGridLayout * topLayout = new QGridLayout(this);
	QGridLayout * oscLayout = new QGridLayout;

	//allocate!
	mDelay = new DelayView;
	mFilter = new FilterView;
	mVCA = new VCAView;
	mEnv3 = new Env3View;
	mFeedback = new FeedbackView;
	mMiscAudio = new MiscAudioView;

	//set up the oscs
	for(unsigned int i = 0; i < 2; i++){
		DigitalOscView * d = new DigitalOscView(this);
		AnalogOscView * a = new AnalogOscView(this);
		oscLayout->addWidget(a, 1, 1 + i);
		oscLayout->addWidget(d, 3, 1 + i);
		mDigitalOscs.push_back(d);
		mAnalogOscs.push_back(a);
		a->show_labels(false);
		d->show_labels(false);
	}
	//don't show the sync button on the last analog osc
	mAnalogOscs.back()->show_sync_button(false);

	//create the labels
	QVBoxLayout * aLabels = new QVBoxLayout;
	for(std::vector<QLabel *>::iterator it = mAnalogOscs[0]->labels()->begin(); 
			it!= mAnalogOscs[0]->labels()->end(); it++){
		aLabels->addWidget(new QLabel(QString((*it)->text()), this), 0, Qt::AlignRight);
	}
	oscLayout->addLayout(aLabels, 1, 0, Qt::AlignRight);

	QVBoxLayout * dLabels = new QVBoxLayout;
	for(std::vector<QLabel *>::iterator it = mDigitalOscs[0]->labels()->begin(); 
			it!= mDigitalOscs[0]->labels()->end(); it++){
		dLabels->addWidget(new QLabel(QString((*it)->text()), this), 0, Qt::AlignRight);
	}
	oscLayout->addLayout(dLabels, 3, 0, Qt::AlignRight);

	oscLayout->addWidget(new QLabel("analog osc 1", this), 0, 1, Qt::AlignCenter);
	oscLayout->addWidget(new QLabel("analog osc 2", this), 0, 2, Qt::AlignCenter);
	oscLayout->addWidget(new QLabel("digital osc 1", this), 2, 1, Qt::AlignCenter);
	oscLayout->addWidget(new QLabel("digital osc 2", this), 2, 2, Qt::AlignCenter);
	oscLayout->setRowStretch(10,1);

	QWidget * oscs = new QWidget(this);
	oscs->setLayout(oscLayout);

	//create a layout for the far right pane
	QVBoxLayout * delay_feedback_and_misc_layout = new QVBoxLayout;
	delay_feedback_and_misc_layout->addWidget(new TitledWidget(QString("delay"), mDelay, this));
	delay_feedback_and_misc_layout->addWidget(new TitledWidget(QString("feedback"), mFeedback, this));
	delay_feedback_and_misc_layout->addWidget(new TitledWidget(QString("misc"), mMiscAudio, this));
	delay_feedback_and_misc_layout->addStretch(10);
	delay_feedback_and_misc_layout->setContentsMargins(1,1,1,1);
	delay_feedback_and_misc_layout->setSpacing(1);
	QWidget * delay_feedback_and_misc_widget = new QWidget(this);
	delay_feedback_and_misc_widget->setLayout(delay_feedback_and_misc_layout);

	//set up the splitters
	QSplitter * topLeftHorizSplit = new QSplitter(Qt::Horizontal, this);
	QSplitter * bottomLeftHorizSplit = new QSplitter(Qt::Horizontal, this);
	QSplitter * leftVertSplit = new QSplitter(Qt::Vertical, this);
	QSplitter * masterHorizSplit = new QSplitter(Qt::Horizontal, this);

	topLeftHorizSplit->addWidget(oscs);
	topLeftHorizSplit->addWidget(new TitledWidget(QString("filter"), mFilter, this));

	//add an lfo?
	bottomLeftHorizSplit->addWidget(new TitledWidget(QString("vca"), mVCA, this));
	bottomLeftHorizSplit->addWidget(new TitledWidget(QString("env 3"), mEnv3, this));

	leftVertSplit->addWidget(topLeftHorizSplit);
	leftVertSplit->addWidget(bottomLeftHorizSplit);

	masterHorizSplit->addWidget(leftVertSplit);
	masterHorizSplit->addWidget(delay_feedback_and_misc_widget);

	topLayout->addWidget(masterHorizSplit);

	setLayout(topLayout);
	topLayout->setContentsMargins(1,1,1,1);
}

std::vector<AnalogOscView *> * AudioAndEnvelopeView::analog_oscs(){
	return &mAnalogOscs;
}

std::vector<DigitalOscView *> * AudioAndEnvelopeView::digital_oscs(){
	return &mDigitalOscs;
}

DelayView * AudioAndEnvelopeView::delay(){
	return mDelay;
}

FilterView * AudioAndEnvelopeView::filter(){
	return mFilter;
}

VCAView * AudioAndEnvelopeView::vca(){
	return mVCA;
}

Env3View * AudioAndEnvelopeView::env3(){
	return mEnv3;
}

FeedbackView * AudioAndEnvelopeView::feedback(){
	return mFeedback;
}

MiscAudioView * AudioAndEnvelopeView::misc_audio(){
	return mMiscAudio;
}

void AudioAndEnvelopeView::connect_to_model(ApplicationModel * model){
	mAnalogOscs[0]->connect_to_model(model->analog_oscs()->front());
	mAnalogOscs[1]->connect_to_model(model->analog_oscs()->back());
	mDigitalOscs[0]->connect_to_model(model->digital_oscs()->front());
	mDigitalOscs[1]->connect_to_model(model->digital_oscs()->back());
	mDelay->connect_to_model(model->delay());
	mFilter->connect_to_model(model->filter());
	mVCA->connect_to_model(model->vca());
	mEnv3->connect_to_model(model->env3());
	mFeedback->connect_to_model(model->feedback());
	mMiscAudio->connect_to_model(model->misc_audio());
}

