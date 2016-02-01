/********************************************************************************
** Form generated from reading UI file 'qtdatagenerator.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTDATAGENERATOR_H
#define UI_QTDATAGENERATOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "cvimagewidget.h"

QT_BEGIN_NAMESPACE

class Ui_QtDataGeneratorClass
{
public:
    QWidget *centralWidget;
    QPushButton *btn_1;
    QLabel *lbl_1;
    CVImageWidget *imageWidget;
    QGraphicsView *pictureBox;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtDataGeneratorClass)
    {
        if (QtDataGeneratorClass->objectName().isEmpty())
            QtDataGeneratorClass->setObjectName(QStringLiteral("QtDataGeneratorClass"));
        QtDataGeneratorClass->resize(715, 582);
        centralWidget = new QWidget(QtDataGeneratorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        btn_1 = new QPushButton(centralWidget);
        btn_1->setObjectName(QStringLiteral("btn_1"));
        btn_1->setGeometry(QRect(60, 100, 161, 101));
        lbl_1 = new QLabel(centralWidget);
        lbl_1->setObjectName(QStringLiteral("lbl_1"));
        lbl_1->setGeometry(QRect(70, 230, 171, 91));
        lbl_1->setAlignment(Qt::AlignCenter);
        imageWidget = new CVImageWidget(centralWidget);
        imageWidget->setObjectName(QStringLiteral("imageWidget"));
        imageWidget->setGeometry(QRect(380, 20, 261, 251));
        pictureBox = new QGraphicsView(centralWidget);
        pictureBox->setObjectName(QStringLiteral("pictureBox"));
        pictureBox->setGeometry(QRect(50, 320, 256, 192));
        QtDataGeneratorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtDataGeneratorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 715, 21));
        QtDataGeneratorClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(QtDataGeneratorClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        QtDataGeneratorClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(QtDataGeneratorClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        QtDataGeneratorClass->setStatusBar(statusBar);

        retranslateUi(QtDataGeneratorClass);

        QMetaObject::connectSlotsByName(QtDataGeneratorClass);
    } // setupUi

    void retranslateUi(QMainWindow *QtDataGeneratorClass)
    {
        QtDataGeneratorClass->setWindowTitle(QApplication::translate("QtDataGeneratorClass", "QtDataGenerator", 0));
        btn_1->setText(QApplication::translate("QtDataGeneratorClass", "Click Me", 0));
        lbl_1->setText(QApplication::translate("QtDataGeneratorClass", "Resting", 0));
    } // retranslateUi

};

namespace Ui {
    class QtDataGeneratorClass: public Ui_QtDataGeneratorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTDATAGENERATOR_H
