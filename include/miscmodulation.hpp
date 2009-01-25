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
	public slots:
		void set_osc_slop(int value);
		void set_mod_amount(unsigned int index, int value);
		void set_mod_destination(unsigned int index, int value);
	signals:
		void osc_slop_changed(int);
		void mod_amount_changed(unsigned int index, int value);
		void mod_destination_changed(unsigned int index, int value);
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
	private slots:
		void update_mod_amount(int index);
		void update_mod_dest(int index);
	public slots:
		void set_osc_slop(int value);
		void set_mod_amount(unsigned int index, int value);
		void set_mod_destination(unsigned int index, int value);
	signals:
		void osc_slop_changed(int);
		void mod_amount_changed(unsigned int index, int value);
		void mod_destination_changed(unsigned int index, int value);
	private:
		QGridLayout * mLayout;
		SliderSpinBox * mOscSlop;

		std::vector<SliderSpinBox *> mAmounts;
		std::vector<ModDestComboBox *> mDestinations;
};

#endif
