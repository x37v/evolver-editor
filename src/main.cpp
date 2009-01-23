#include "oscmodel.hpp"
#include "oscview.hpp"
#include "lfo.hpp"
#include "delay.hpp"
#include "vca.hpp"
#include "env3.hpp"
#include <QApplication>
#include "modulation_destinations.hpp"
#include "applicationmodel.hpp"
#include "applicationview.hpp"
#include "audioandenvelopeview.hpp"
#include "modulationview.hpp"
#include "modulators.hpp"
#include "miscaudio.hpp"

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);
	ApplicationModel * model = new ApplicationModel(&app);
	ApplicationView * view = new ApplicationView();
	view->connect_to_model(model);

	//view->modulation()->lfos()->front()->connect_to_model(model->lfos()->front());
	//model->lfos()->front()->set_destination(5);
	/*
	view->audio_and_envelopes()->analog_oscs()->front()->connect_to_model(model->analog_oscs()->front());
	model->analog_oscs()->front()->set_freq(100);

	view->audio_and_envelopes()->digital_oscs()->front()->connect_to_model(model->digital_oscs()->front());
	model->digital_oscs()->front()->set_shape(100);

	view->audio_and_envelopes()->delay()->connect_to_model(model->delay());
	model->delay()->set_feedback_level(25);
	model->delay()->set_delay_sync(1,3);

	view->audio_and_envelopes()->vca()->connect_to_model(model->vca());
	model->vca()->set_pan(3);
	model->vca()->set_velocity(25);

	view->audio_and_envelopes()->env3()->connect_to_model(model->env3());
	model->env3()->set_velocity(25);
	
	view->audio_and_envelopes()->misc_audio()->connect_to_model(model->misc_audio());
	model->misc_audio()->set_volume(25);
	*/
	
	//AudioAndEnvelopeView * view = new AudioAndEnvelopeView();
	//ModulationView * view = new ModulationView();
	
	view->show();
   return app.exec();
}

