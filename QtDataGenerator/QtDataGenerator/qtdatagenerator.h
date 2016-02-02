#ifndef QTDATAGENERATOR_H
#define QTDATAGENERATOR_H

#include <QtWidgets/QMainWindow>
#include "ui_qtdatagenerator.h"
#include <opencv2\opencv.hpp>
#include <Qfiledialog.h>
//#include <QDialog>
#include <QPainter>
//#include <QRect>
//#include <QBrush>
//#include <QApplication>
#include <QMouseEvent>
#include <QCursor>

class QtDataGenerator : public QMainWindow
{
	Q_OBJECT

public:
	QtDataGenerator(QWidget *parent = 0);
	~QtDataGenerator();
	void QtDataGenerator::mouseMoveEvent(QMouseEvent *ev);

private slots:
void on_btn_1_clicked();
void on_btn_browser_clicked();
private:
	Ui::QtDataGeneratorClass ui;

protected: 
	void paintEvent(QPaintEvent *e);

};

#endif // QTDATAGENERATOR_H
