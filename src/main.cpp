/*
 *		Copyright (c) 2009 Alex Norman.  All rights reserved.
 *		http://www.x37v.info/
 *
 *		This file is part of Alex's Evolver Editor.
 *		
 *		This Evolver Editor is free software: you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License as published
 *		by the Free Software Foundation, either version 3 of the License, or (at
 *		your option) any later version.
 *		
 *		This Evolver Editor is distributed in the hope that it will be useful,
 *		but WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
 *		Public License for more details.
 *		
 *		You should have received a copy of the GNU General Public License along
 *		with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "oscmodel.hpp"
#include "oscview.hpp"
#include "lfo.hpp"
#include "delay.hpp"
#include "filter.hpp"
#include "vca.hpp"
#include "env3.hpp"
#include "modulation_destinations.hpp"
#include "applicationmodel.hpp"
#include "applicationview.hpp"
#include "audioandenvelopeview.hpp"
#include "modulationview.hpp"
#include "modulators.hpp"
#include "miscaudio.hpp"
#include "miscmodulation.hpp"
#include "mididriver.hpp"
#include "mididriverview.hpp"

#include <QApplication>
#include <QFile>
#include <QAction>
#include <QSignalMapper>
#include <QKeySequence>
#include <QObject>
#include <map>
#include <iostream>
using std::cout;
using std::endl;

#include <boost/program_options.hpp>
namespace po = boost::program_options;

//print out a list of the midi inputs and outputs
void print_midi_io(MidiDriver * driver){
	const std::map<unsigned int, QString> * i = driver->input_map();
	const std::map<unsigned int, QString> * o = driver->output_map();

	cout << "Midi Input Devices" << endl;
	cout << "\tindex\tname" << endl;
	for(std::map<unsigned int, QString>::const_iterator it = i->begin(); it != i->end(); it++)
		cout << "\t" << it->first << "\t" << it->second.toStdString() << endl;

	cout << endl << "Midi Output Devices" << endl;
	cout << "\tindex\tname" << endl;
	for(std::map<unsigned int, QString>::const_iterator it = o->begin(); it != o->end(); it++)
		cout << "\t" << it->first << "\t" << it->second.toStdString() << endl;
}


int main(int argc, char *argv[])
{
   QApplication app(argc, argv);
	ApplicationModel * model = new ApplicationModel(&app);
	ApplicationView * view = new ApplicationView();
	MidiDriver * driver = new MidiDriver(model, model);
	model->set_midi_driver(driver);
	view->connect_to_model(model);

	//update the list of midi i/o devices
	driver->update_device_list();
	view->midi_driver()->update_inputs(driver->input_map());
	view->midi_driver()->update_outputs(driver->output_map());

	//connect up the driver and its view
	QObject::connect(
			driver, 
			SIGNAL(input_index_changed(int)),
			view->midi_driver(), 
			SLOT(set_input_index(int)));
	QObject::connect(
			view->midi_driver(), 
			SIGNAL(input_index_changed(int)),
			driver, 
			SLOT(open_input(int)));
	QObject::connect(
			driver, 
			SIGNAL(output_index_changed(int)),
			view->midi_driver(), 
			SLOT(set_output_index(int)));
	QObject::connect(
			view->midi_driver(), 
			SIGNAL(output_index_changed(int)),
			driver, 
			SLOT(open_output(int)));


	try {
		// Declare the supported options.
		po::options_description desc("Evolver Editor options");
		po::variables_map vm;        
		po::positional_options_description p;

		desc.add_options()
			("help,h", "Print this help message.")
			("list-midiio,l", "List the midi input and output devices that we can connect to.")
			("midiin,i", po::value<unsigned int>(), "Specify a port number to use for MIDI input messages.")
			("midiin-name,n", po::value<std::string>(), "Specify a port name to use for MIDI input messages.")
			("midiout,o", po::value<unsigned int>(), "Specify a port number to use for MIDI output messages.")
			("midiout-name,m", po::value<std::string>(), "Specify a port name to use for MIDI output messages.")
			("style-sheet,s", po::value<std::string>(), "Specify a style sheet file to use for the GUI.")
			;

		po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
		po::notify(vm);    

		if (vm.count("help")) {
			cout << endl << desc << endl;
			return 0;
		}
		if (vm.count("list-midiio")) {
			cout << endl;
			print_midi_io(driver);
			return 0;
		}

		//connect our midi i/o
		if(vm.count("midiin")){
			if(driver->input_map()->find(vm["midiin"].as<unsigned int>()) == driver->input_map()->end()){
				cout << endl << "Midi input device select is not valid" << endl << endl;
				cout << "Select from one of the midi input indices below" << endl << endl;
				print_midi_io(driver);
				return -1;
			} else
				driver->open_input(vm["midiin"].as<unsigned int>());
		}
		if(vm.count("midiin-name"))
			driver->open_input(QString(vm["midiin-name"].as<std::string>().c_str()));

		if(vm.count("midiout")){
			if(driver->output_map()->find(vm["midiout"].as<unsigned int>()) == driver->output_map()->end()){
				cout << endl << "Midi output device select is not valid" << endl << endl;
				cout << "Select from one of the midi output indices below" << endl << endl;
				print_midi_io(driver);
				return -1;
			} else
				driver->open_output(vm["midiout"].as<unsigned int>());
		} 
		if(vm.count("midiout-name"))
			driver->open_output(QString(vm["midiout-name"].as<std::string>().c_str()));

		//set the style sheet
		if(vm.count("style-sheet")){
			QFile file(QString(vm["style-sheet"].as<std::string>().c_str()));
			if(file.open(QFile::ReadOnly)){
				QString styleSheet = QLatin1String(file.readAll());
				app.setStyleSheet(styleSheet);
			} else {
				qWarning("Cannot open style sheet: %s\n"
						"Application will use default style.", vm["style-sheet"].as<std::string>().c_str());
			}
		} 
	} catch(std::exception& e) {
		std::string str("Error in command line arguments: ");
		str.append(e.what());
		qFatal(str.c_str());
		return app.exec();
	}

	//stop the thread when we're gonna quit
	QObject::connect(&app,
			SIGNAL(aboutToQuit()),
			driver,
			SLOT(quit()));
	driver->start();

	//set up key bindings
	
	//control+r requests edit buffer
	QAction * action = new QAction(view);
	action->setShortcut(QKeySequence(QString("Ctrl+r")));
	QObject::connect(action, SIGNAL(triggered()), driver, SLOT(request_update_state()));
	view->addAction(action);

	//control+num goes to that tab
	QSignalMapper * mapper = new QSignalMapper(view);
	for(int i = 0; i < view->tab_widget()->count(); i++){
		action = new QAction(view);
		action->setShortcut(QKeySequence(QString("Ctrl+%1").arg(i + 1)));
		QObject::connect(action, SIGNAL(triggered()), mapper, SLOT(map()));
		mapper->setMapping(action, i);
		QObject::connect(mapper, SIGNAL(mapped(int)), view->tab_widget(), SLOT(setCurrentIndex(int)));
		view->addAction(action);
	}
	
	view->show();
   return app.exec();
}

