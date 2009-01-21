#include "applicationmodel.hpp"
#include "lfo.hpp"
#include "oscmodel.hpp"

ApplicationModel::ApplicationModel(QObject * parent) : QObject(parent){
	for(unsigned int i = 0; i < 4; i++)
		mLFOs.push_back(new LFOModel(this));
	for(unsigned int i = 0; i < 2; i++){
		mDigitalOscs.push_back(new DigitalOscModel(this));
		mAnalogOscs.push_back(new AnalogOscModel(this));
	}
}

