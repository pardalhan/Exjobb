/********************************************************************************
** Form generated from reading UI file 'gridface.ui'
**
** Created by: Qt User Interface Compiler version 5.6.0
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
    QPushButton *btn_next_run;
    QLabel *lbl_msg;
    QPushButton *btn_browser;
    QPushButton *btn_train;
    QPushButton *btn_validate;
    QLabel *lbl_browser;
    QLabel *lbl_result;
    QPushButton *btn_next_validate;
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
        btn_next_run = new QPushButton(centralWidget);
        btn_next_run->setObjectName(QStringLiteral("btn_next_run"));
        btn_next_run->setGeometry(QRect(240, 90, 151, 71));
        lbl_msg = new QLabel(centralWidget);
        lbl_msg->setObjectName(QStringLiteral("lbl_msg"));
        lbl_msg->setGeometry(QRect(250, 190, 331, 151));
        lbl_msg->setAlignment(Qt::AlignCenter);
        btn_browser = new QPushButton(centralWidget);
        btn_browser->setObjectName(QStringLiteral("btn_browser"));
        btn_browser->setGeometry(QRect(240, 10, 151, 71));
        btn_train = new QPushButton(centralWidget);
        btn_train->setObjectName(QStringLiteral("btn_train"));
        btn_train->setGeometry(QRect(480, 10, 131, 61));
        btn_validate = new QPushButton(centralWidget);
        btn_validate->setObjectName(QStringLiteral("btn_validate"));
        btn_validate->setGeometry(QRect(30, 10, 121, 71));
        lbl_browser = new QLabel(centralWidget);
        lbl_browser->setObjectName(QStringLiteral("lbl_browser"));
        lbl_browser->setGeometry(QRect(450, 70, 161, 51));
        lbl_browser->setAlignment(Qt::AlignCenter);
        lbl_result = new QLabel(centralWidget);
        lbl_result->setObjectName(QStringLiteral("lbl_result"));
        lbl_result->setGeometry(QRect(40, 170, 151, 211));
        lbl_result->setAlignment(Qt::AlignCenter);
        btn_next_validate = new QPushButton(centralWidget);
        btn_next_validate->setObjectName(QStringLiteral("btn_next_validate"));
        btn_next_validate->setEnabled(false);
        btn_next_validate->setGeometry(QRect(30, 90, 121, 81));
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
        btn_next_run->setText(QApplication::translate("GridFaceClass", "Next run", 0));
        lbl_msg->setText(QApplication::translate("GridFaceClass", "Message", 0));
        btn_browser->setText(QApplication::translate("GridFaceClass", "Run", 0));
        btn_train->setText(QApplication::translate("GridFaceClass", "Train", 0));
        btn_validate->setText(QApplication::translate("GridFaceClass", "Validate", 0));
        lbl_browser->setText(QApplication::translate("GridFaceClass", "Directory", 0));
        lbl_result->setText(QApplication::translate("GridFaceClass", "Results", 0));
        btn_next_validate->setText(QApplication::translate("GridFaceClass", "Next validate", 0));
    } // retranslateUi

};

namespace Ui {
    class GridFaceClass: public Ui_GridFaceClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GRIDFACE_H
