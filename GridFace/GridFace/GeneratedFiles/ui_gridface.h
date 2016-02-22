/********************************************************************************
** Form generated from reading UI file 'gridface.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GRIDFACE_H
#define UI_GRIDFACE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GridFaceClass
{
public:
    QWidget *centralWidget;
    QPushButton *btn_run;
    QLabel *lbl_msg;
    QPushButton *btn_browser;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GridFaceClass)
    {
        if (GridFaceClass->objectName().isEmpty())
            GridFaceClass->setObjectName(QStringLiteral("GridFaceClass"));
        GridFaceClass->resize(627, 451);
        centralWidget = new QWidget(GridFaceClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        btn_run = new QPushButton(centralWidget);
        btn_run->setObjectName(QStringLiteral("btn_run"));
        btn_run->setGeometry(QRect(180, 10, 281, 51));
        lbl_msg = new QLabel(centralWidget);
        lbl_msg->setObjectName(QStringLiteral("lbl_msg"));
        lbl_msg->setGeometry(QRect(50, 190, 521, 151));
        lbl_msg->setAlignment(Qt::AlignCenter);
        btn_browser = new QPushButton(centralWidget);
        btn_browser->setObjectName(QStringLiteral("btn_browser"));
        btn_browser->setGeometry(QRect(240, 80, 171, 41));
        GridFaceClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(GridFaceClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 627, 21));
        GridFaceClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(GridFaceClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        GridFaceClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(GridFaceClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        GridFaceClass->setStatusBar(statusBar);

        retranslateUi(GridFaceClass);

        QMetaObject::connectSlotsByName(GridFaceClass);
    } // setupUi

    void retranslateUi(QMainWindow *GridFaceClass)
    {
        GridFaceClass->setWindowTitle(QApplication::translate("GridFaceClass", "GridFace", 0));
        btn_run->setText(QApplication::translate("GridFaceClass", "RUN FOR THE HILLS", 0));
        lbl_msg->setText(QApplication::translate("GridFaceClass", "Message", 0));
        btn_browser->setText(QApplication::translate("GridFaceClass", "Browser", 0));
    } // retranslateUi

};

namespace Ui {
    class GridFaceClass: public Ui_GridFaceClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRIDFACE_H
