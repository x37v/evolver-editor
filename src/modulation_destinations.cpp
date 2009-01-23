#include "modulation_destinations.hpp"

ModDestComboBox::ModDestComboBox(QWidget * parent) : QComboBox(parent){
	for(unsigned int i = 0; i < NUM_MODULATION_DESTINATIONS; i++)
		addItem(modulation_destinations[i]);
}

