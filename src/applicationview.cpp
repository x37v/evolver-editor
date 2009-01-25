#include "applicationview.hpp"
#include "applicationmodel.hpp"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include "audioandenvelopeview.hpp"
#include "modulationview.hpp"

ApplicationView::ApplicationView(QWidget * parent) : QWidget(parent){
	//allocate
	mLayout = new QVBoxLayout(this);
	mTabView = new QTabWidget(this);
	mAudioAndEnvelopes = new AudioAndEnvelopeView(this);
	mModulations = new ModulationView(this);

	QScrollArea * tab0 = new QScrollArea(this);
	QScrollArea * tab1 = new QScrollArea(this);

	tab0->setWidget(mAudioAndEnvelopes);
	tab1->setWidget(mModulations);

	tab0->setWidgetResizable(true);
	tab1->setWidgetResizable(true);

	//add tabs and layout
	mTabView->addTab(tab0, QString("audio and envelopes"));
	mTabView->addTab(tab1, QString("modulations"));

	mLayout->addWidget(mTabView);
	mLayout->setContentsMargins(1,1,1,1);
	setLayout(mLayout);
}

AudioAndEnvelopeView * ApplicationView::audio_and_envelopes(){
	return mAudioAndEnvelopes;
}

ModulationView * ApplicationView::modulation(){
	return mModulations;
}

void ApplicationView::connect_to_model(ApplicationModel * model){
	mAudioAndEnvelopes->connect_to_model(model);
	mModulations->connect_to_model(model);
}

