/********************************************************************************
** Form generated from reading UI file 'facegrid.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FACEGRID_H
#define UI_FACEGRID_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FaceGridClass
{
public:
    QWidget *centralWidget;
    QPushButton *btn_run;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FaceGridClass)
    {
        if (FaceGridClass->objectName().isEmpty())
            FaceGridClass->setObjectName(QStringLiteral("FaceGridClass"));
        FaceGridClass->resize(335, 252);
        centralWidget = new QWidget(FaceGridClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        btn_run = new QPushButton(centralWidget);
        btn_run->setObjectName(QStringLiteral("btn_run"));
        btn_run->setGeometry(QRect(70, 50, 201, 101));
        FaceGridClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(FaceGridClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 335, 26));
        FaceGridClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FaceGridClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FaceGridClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(FaceGridClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FaceGridClass->setStatusBar(statusBar);

        retranslateUi(FaceGridClass);

        QMetaObject::connectSlotsByName(FaceGridClass);
    } // setupUi

    void retranslateUi(QMainWindow *FaceGridClass)
    {
        FaceGridClass->setWindowTitle(QApplication::translate("FaceGridClass", "FaceGrid", 0));
        btn_run->setText(QApplication::translate("FaceGridClass", "RUN FOR THE HILLS", 0));
    } // retranslateUi

};

namespace Ui {
    class FaceGridClass: public Ui_FaceGridClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FACEGRID_H
