#include "qtdatagenerator.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtDataGenerator w;
	w.show();
	return a.exec();
}
