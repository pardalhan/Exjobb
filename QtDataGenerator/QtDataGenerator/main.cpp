#include "qtdatagenerator.h"
#include <QtWidgets/QApplication>
#include <iostream>

int main(int argc, char *argv[])
{
	std::cout << "hej" << std::endl;
	QApplication a(argc, argv);
	QtDataGenerator w;
	w.show();
	return a.exec();
}
