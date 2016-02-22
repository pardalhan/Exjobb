#include "gridface.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	GridFace w;
	w.show();
	return a.exec();
}
