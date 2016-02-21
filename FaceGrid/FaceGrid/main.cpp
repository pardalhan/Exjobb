#include "facegrid.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	FaceGrid w;
	w.show();
	return a.exec();
}
