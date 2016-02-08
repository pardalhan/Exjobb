#ifndef MYIMAGE_H
#define MYIMAGE_H

#include <qstring.h>
class MyImage
{

 
public:
	MyImage();
	~MyImage();
	QString get_filename();
	QString set_filename(QString name);

private: 
	QString filename; 
};

#endif