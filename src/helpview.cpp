#include "helpview.hpp"
#include <QTextStream>
#include <QString>
#include <QFile>

HelpView::HelpView(QWidget * parent) : QTextEdit(parent) {
	setReadOnly(true);

	QFile help_file("HELP");
	if(help_file.open(QIODevice::ReadOnly | QIODevice::Text)){
		QTextStream help_file_stream(&help_file);
		setText(help_file_stream.readAll());
	} else {
		setText("help file not found");
	}

	help_file.close();
}
