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
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtDataGeneratorClass
{
public:
    QWidget *centralWidget;
    QPushButton *btn_next;
    QLabel *lbl_img;
    QLabel *lbl_msg;
    QPushButton *btn_browser;
    QPushButton *btn_zoomin;
    QPushButton *btn_zoomout;
    QPushButton *btn_clearall;
    QPushButton *btn_clearlast;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtDataGeneratorClass)
    {
        if (QtDataGeneratorClass->objectName().isEmpty())
            QtDataGeneratorClass->setObjectName(QStringLiteral("QtDataGeneratorClass"));
        QtDataGeneratorClass->resize(799, 714);
        centralWidget = new QWidget(QtDataGeneratorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        btn_next = new QPushButton(centralWidget);
        btn_next->setObjectName(QStringLiteral("btn_next"));
        btn_next->setEnabled(false);
        btn_next->setGeometry(QRect(30, 290, 181, 111));
        lbl_img = new QLabel(centralWidget);
        lbl_img->setObjectName(QStringLiteral("lbl_img"));
        lbl_img->setGeometry(QRect(230, 20, 541, 591));
        lbl_img->setLayoutDirection(Qt::LeftToRight);
        lbl_img->setAlignment(Qt::AlignCenter);
        lbl_msg = new QLabel(centralWidget);
        lbl_msg->setObjectName(QStringLiteral("lbl_msg"));
        lbl_msg->setGeometry(QRect(40, 420, 181, 91));
        lbl_msg->setAlignment(Qt::AlignCenter);
        btn_browser = new QPushButton(centralWidget);
        btn_browser->setObjectName(QStringLiteral("btn_browser"));
        btn_browser->setGeometry(QRect(50, 570, 161, 51));
        btn_zoomin = new QPushButton(centralWidget);
        btn_zoomin->setObjectName(QStringLiteral("btn_zoomin"));
        btn_zoomin->setEnabled(false);
        btn_zoomin->setGeometry(QRect(50, 30, 151, 51));
        btn_zoomin->setCheckable(true);
        btn_zoomin->setChecked(false);
        btn_zoomout = new QPushButton(centralWidget);
        btn_zoomout->setObjectName(QStringLiteral("btn_zoomout"));
        btn_zoomout->setEnabled(false);
        btn_zoomout->setGeometry(QRect(50, 90, 151, 51));
        btn_clearall = new QPushButton(centralWidget);
        btn_clearall->setObjectName(QStringLiteral("btn_clearall"));
        btn_clearall->setEnabled(false);
        btn_clearall->setGeometry(QRect(50, 150, 151, 61));
        btn_clearlast = new QPushButton(centralWidget);
        btn_clearlast->setObjectName(QStringLiteral("btn_clearlast"));
        btn_clearlast->setEnabled(false);
        btn_clearlast->setGeometry(QRect(50, 220, 151, 61));
        QtDataGeneratorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtDataGeneratorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 799, 26));
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
        btn_next->setText(QApplication::translate("QtDataGeneratorClass", "NEXT IMAGE", 0));
        lbl_img->setText(QApplication::translate("QtDataGeneratorClass", "Resting", 0));
        lbl_msg->setText(QApplication::translate("QtDataGeneratorClass", "Message", 0));
        btn_browser->setText(QApplication::translate("QtDataGeneratorClass", "Browser", 0));
        btn_zoomin->setText(QApplication::translate("QtDataGeneratorClass", "Zoom in", 0));
        btn_zoomout->setText(QApplication::translate("QtDataGeneratorClass", "Zoom out", 0));
        btn_clearall->setText(QApplication::translate("QtDataGeneratorClass", "Clear all", 0));
        btn_clearlast->setText(QApplication::translate("QtDataGeneratorClass", "Clear last", 0));
    } // retranslateUi

};

namespace Ui {
    class QtDataGeneratorClass: public Ui_QtDataGeneratorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTDATAGENERATOR_H
