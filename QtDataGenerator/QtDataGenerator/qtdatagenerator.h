#ifndef QTDATAGENERATOR_H
#define QTDATAGENERATOR_H

#include <QtWidgets/QMainWindow>
#include "ui_qtdatagenerator.h"
#include <opencv2\opencv.hpp>

class QtDataGenerator : public QMainWindow
{
	Q_OBJECT

public:
	QtDataGenerator(QWidget *parent = 0);
	~QtDataGenerator();

private slots:
void on_btn_1_clicked();

private:
	Ui::QtDataGeneratorClass ui;
};

#endif // QTDATAGENERATOR_H
