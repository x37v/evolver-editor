#ifndef MISC_MODULATION_HPP
#define MISC_MODULATION_HPP

#include <QObject>
#include <vector>

class MiscModulationModel : public QObject {
	Q_OBJECT
	public:
		MiscModulationModel(QObject * parent = NULL);
		enum modulation_source {
			in_peak,
			in_env_follow,
			velocity,
			mod_wheel,
			pressure,
			breath,
			foot
		};
		const static char * modulation_source_names[];
		const static unsigned int num_modulation_sources;
		const static unsigned int osc_slop_max;
		const static int amount_min;
		const static int amount_max;
	private:
		unsigned int mOscSlop;

		std::vector<int> mAmounts;
		std::vector<unsigned int> mDestinations;
};

#include <QWidget>
class QComboBox;
class SliderSpinBox;
class QGridLayout;
class ModDestComboBox;

class MiscModulationView : public QWidget {
	Q_OBJECT
	public:
		MiscModulationView(QWidget * parent = NULL);
		void connect_to_model(MiscModulationModel * model);
	private:
		QGridLayout * mLayout;
		SliderSpinBox * mOscSlop;

		std::vector<SliderSpinBox *> mAmounts;
		std::vector<ModDestComboBox *> mDestinations;
};

#endif
