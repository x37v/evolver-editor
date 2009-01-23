#ifndef MODULATION_ROUTING_HPP
#define MODULATION_ROUTING_HPP

#include <QWidget>
class QLabel;
class QComboBox;
class SliderSpinBox;
class QGridLayout;
class ModDestComboBox;

class ModRoutingModel : public QObject {
	Q_OBJECT
	public:
		ModRoutingModel(QObject * parent = NULL);
		const static int amount_max;
		const static int amount_min;
	signals:
		void amount_changed(int);
		void destination_changed(int);
		void source_changed(int);
	public slots:
		void set_amount(int amount);
		void set_source(int source);
		void set_destination(int dest);
	private:
		int mAmount;
		unsigned int mSource;
		unsigned int mDestination;
};

class ModRoutingView : public QWidget {
	Q_OBJECT
	public:
		ModRoutingView(QWidget * parent = NULL);
		std::vector<QLabel *> * labels();
		void connect_to_model(ModRoutingModel * model);
	signals:
		void amount_changed(int);
		void destination_changed(int);
		void source_changed(int);
	public slots:
		void show_labels(bool show);
		void set_amount(int amount);
		void set_source(int source);
		void set_destination(int dest);
	private:
		std::vector<QLabel *> mLabels;
		SliderSpinBox * mAmountSlider;
		QComboBox * mSrcSelect;
		ModDestComboBox * mDestSelect;
		QGridLayout * mLayout;

};

#endif
