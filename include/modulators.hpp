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
};

class ModRoutingView : public QWidget {
	Q_OBJECT
	public:
		ModRoutingView(QWidget * parent = NULL);
		std::vector<QLabel *> * labels();
	public slots:
		void show_labels(bool show);
	private:
		std::vector<QLabel *> mLabels;
		SliderSpinBox * mAmountSlider;
		QComboBox * mSrcSelect;
		ModDestComboBox * mDestSelect;
		QGridLayout * mLayout;

};

#endif
