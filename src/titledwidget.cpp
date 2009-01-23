#include "titledwidget.hpp"
#include <QLabel>
#include <QVBoxLayout>

TitledWidget::TitledWidget(QString title, QWidget * widget, QWidget * parent) : QWidget(parent){
	mLayout = new QVBoxLayout(this);
	mLabel = new QLabel(title, this);

	mWidget = widget;
	widget->setParent(this);
	mLayout->addWidget(mLabel, 0, Qt::AlignHCenter);
	mLayout->addWidget(mWidget, 0);
	mLayout->setContentsMargins(1,1,1,1);
	mLayout->setSpacing(2);
	mLayout->addStretch(1);

	setLayout(mLayout);
}

