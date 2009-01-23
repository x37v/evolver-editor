#ifndef TITLED_WIDGET_HPP
#define TITLED_WIDGET_HPP

#include <QWidget>
#include <QString>

class QVBoxLayout;
class QLabel;

//this widget is simply another widget with a QLabel at the top

class TitledWidget : public QWidget {
	Q_OBJECT
	public:
		TitledWidget(QString title, QWidget * widget, QWidget * parent = NULL);
	private:
		QWidget * mWidget;
		QLabel * mLabel;
		QVBoxLayout * mLayout;
};
#endif
