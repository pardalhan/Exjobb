#include "MyImage.h"


MyImage::MyImage()
{
}


MyImage::~MyImage()
{
}

QString MyImage::get_filename()
{
	return filename; 
}

QString MyImage::set_filename(QString name)
{
	filename = name; 
	return 0; 
}
