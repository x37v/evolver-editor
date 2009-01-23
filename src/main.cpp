#include "oscmodel.hpp"
#include "oscview.hpp"
#include "lfo.hpp"
#include <QApplication>
#include "modulation_destinations.hpp"
#include "applicationmodel.hpp"
#include "applicationview.hpp"
#include "audioandenvelopeview.hpp"
#include "modulationview.hpp"

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);
	ApplicationModel * model = new ApplicationModel(&app);
	//ApplicationView * view = new ApplicationView();
	//AudioAndEnvelopeView * view = new AudioAndEnvelopeView();
	ModulationView * view = new ModulationView();
	
	view->show();
   return app.exec();
}

