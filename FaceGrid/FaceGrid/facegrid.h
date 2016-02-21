#ifndef FACEGRID_H
#define FACEGRID_H

#include <QtWidgets/QMainWindow>
#include "ui_facegrid.h"
#include <opencv2\opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <Qfiledialog.h>
#include <qdebug.h>
#include <qdiriterator.h>
#include <QMouseEvent>
#include <QMessageBox>
#include <vector>
#include <string>
#include <qmessagebox.h>
#include <iostream>
#include <fstream>

#include "flandmark_detector.h"
#include <cstring>
#include <cmath>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"


class FaceGrid : public QMainWindow
{
	Q_OBJECT

public:
	FaceGrid(QWidget *parent = 0);
	~FaceGrid();

private slots:
void on_btn_run_clicked();

private:
	Ui::FaceGridClass ui;
};

#endif // FACEGRID_H
