#ifndef MODULATION_DESTINATIONS_HPP
#define MODULATION_DESTINATIONS_HPP

#define NUM_MODULATION_DESTINATIONS 69

#include <QComboBox>

class ModDestComboBox : public QComboBox {
	Q_OBJECT
	public:
		ModDestComboBox(QWidget * parent = NULL);
		static const char* modulation_destinations[];
};

#endif
