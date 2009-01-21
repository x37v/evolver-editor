#include "oscmodel.hpp"
#include "oscview.hpp"
#include "lfo.hpp"
#include <QApplication>
#include "modulation_destinations.hpp"
#include "applicationmodel.hpp"
#include "applicationview.hpp"

int main(int argc, char *argv[])
{
   QApplication app(argc, argv);
	ApplicationModel * model = new ApplicationModel(&app);
	ApplicationView * view = new ApplicationView();
	//LFOView * view = new LFOView();
	view->show();
   return app.exec();
}

