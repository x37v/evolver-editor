#include "audioandenvelopeview.hpp"
#include "oscview.hpp"
#include "delay.hpp"
#include "filter.hpp"
#include "vca.hpp"
#include "env3.hpp"
#include "feedback.hpp"
#include "miscaudio.hpp"
#include <QSplitter>
#include <QVBoxLayout>
#include <QLabel>
#include "titledwidget.hpp"

AudioAndEnvelopeView::AudioAndEnvelopeView(QWidget * parent) : QWidget(parent){
	QGridLayout * topLayout = new QGridLayout(this);
	QGridLayout * oscLayout = new QGridLayout;

	mDelay = new DelayView;
	mFilter = new FilterView;
	mVCA = new VCAView;
	mEnv3 = new Env3View;
	mFeedback = new FeedbackView;
	mMiscAudio = new MiscAudioView;

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

	QVBoxLayout * delay_feedback_and_env3_layout = new QVBoxLayout;
	delay_feedback_and_env3_layout->addWidget(new TitledWidget(QString("delay"), mDelay, this));
	delay_feedback_and_env3_layout->addWidget(new TitledWidget(QString("feedback"), mFeedback, this));
	delay_feedback_and_env3_layout->addStretch(10);
	delay_feedback_and_env3_layout->setContentsMargins(1,1,1,1);
	delay_feedback_and_env3_layout->setSpacing(1);
	QWidget * delay_feedback_and_env3_widget = new QWidget(this);
	delay_feedback_and_env3_widget->setLayout(delay_feedback_and_env3_layout);

	QVBoxLayout * filt_and_vca_layout = new QVBoxLayout;
	filt_and_vca_layout->addWidget(new TitledWidget(QString("filter"), mFilter, this));
	filt_and_vca_layout->addStretch(10);
	QWidget * filt_and_vca_widget = new QWidget(this);
	filt_and_vca_widget->setLayout(filt_and_vca_layout);

	/*
	filt_and_vca_layout->addWidget(new TitledWidget(QString("vca"), mVCA, this));
	delay_feedback_and_env3_layout->addWidget(new TitledWidget(QString("env 3"), mEnv3, this));
	mMiscAudio
	1 lfo?
	*/

	QSplitter * topSplitter = new QSplitter(Qt::Horizontal, this);
	QSplitter * bottomSplitter = new QSplitter(Qt::Horizontal, this);
	QSplitter * leftSplitter = new QSplitter(Qt::Vertical, this);

	topSplitter->addWidget(oscs);
	topSplitter->addWidget(filt_and_vca_widget);
	topSplitter->addWidget(delay_feedback_and_env3_widget);

	bottomSplitter->addWidget(new TitledWidget(QString("vca"), mVCA, this));
	bottomSplitter->addWidget(new TitledWidget(QString("env 3"), mEnv3, this));
	bottomSplitter->addWidget(new TitledWidget(QString("misc"), mMiscAudio, this));

	leftSplitter->addWidget(topSplitter);
	leftSplitter->addWidget(bottomSplitter);

	topLayout->addWidget(leftSplitter);

	setLayout(topLayout);
	topLayout->setContentsMargins(1,1,1,1);
}
