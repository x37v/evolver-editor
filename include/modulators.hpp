#ifndef MODULATION_ROUTING_HPP
#define MODULATION_ROUTING_HPP

#include <QWidget>
class QLabel;
class QComboBox;
class SliderSpinBox;
class QGridLayout;

class ModModel : public QObject {
	Q_OBJECT
	public:
		const static int amount_max;
		const static int amount_min;
};

class ModView : public QWidget {
	Q_OBJECT
	public:
		ModView(QWidget * parent = NULL);
		std::vector<QLabel *> * labels();
	public slots:
		void show_labels(bool show);
	private:
		std::vector<QLabel *> mLabels;
		SliderSpinBox * mAmountSlider;
		QComboBox * mSrcSelect;
		QComboBox * mDestSelect;
		QGridLayout * mLayout;

};

#endif
