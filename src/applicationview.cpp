#include "applicationview.hpp"
#include "applicationmodel.hpp"
#include <QTabWidget>
#include <QVBoxLayout>
#include "audioandenvelopeview.hpp"
#include "modulationview.hpp"

ApplicationView::ApplicationView(QWidget * parent) : QWidget(parent){
	//allocate
	mLayout = new QVBoxLayout(this);
	mTabView = new QTabWidget(this);
	mAudioAndEnvelopes = new AudioAndEnvelopeView(this);
	mModulations = new ModulationView(this);

	//add tabs and layout
	mTabView->addTab(mAudioAndEnvelopes, QString("audio and envelopes"));
	mTabView->addTab(mModulations, QString("modulations"));

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

