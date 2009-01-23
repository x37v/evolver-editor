#include "oscmodel.hpp"
#include "oscview.hpp"
#include "lfo.hpp"
#include <QApplication>
#include "modulation_destinations.hpp"
#include "applicationmodel.hpp"
#include "applicationview.hpp"
#include "audioandenvelopeview.hpp"
#include "modulationview.hpp"
#include "modulators.hpp"

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);
	ApplicationModel * model = new ApplicationModel(&app);
	ApplicationView * view = new ApplicationView();

	//view->modulation()->lfos()->front()->connect_to_model(model->lfos()->front());
	//model->lfos()->front()->set_destination(5);
	/*
	view->audio_and_envelopes()->analog_oscs()->front()->connect_to_model(model->analog_oscs()->front());
	model->analog_oscs()->front()->set_freq(100);

	view->audio_and_envelopes()->digital_oscs()->front()->connect_to_model(model->digital_oscs()->front());
	model->digital_oscs()->front()->set_shape(100);
	*/
	
	//AudioAndEnvelopeView * view = new AudioAndEnvelopeView();
	//ModulationView * view = new ModulationView();
	
	view->show();
   return app.exec();
}

