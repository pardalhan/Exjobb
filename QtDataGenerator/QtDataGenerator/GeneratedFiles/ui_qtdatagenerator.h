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
    QLabel *lbl_1;
    QLabel *lbl_2;
    QPushButton *btn_browser;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtDataGeneratorClass)
    {
        if (QtDataGeneratorClass->objectName().isEmpty())
            QtDataGeneratorClass->setObjectName(QStringLiteral("QtDataGeneratorClass"));
        QtDataGeneratorClass->resize(735, 701);
        centralWidget = new QWidget(QtDataGeneratorClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        btn_next = new QPushButton(centralWidget);
        btn_next->setObjectName(QStringLiteral("btn_next"));
        btn_next->setEnabled(false);
        btn_next->setGeometry(QRect(40, 180, 161, 101));
        lbl_1 = new QLabel(centralWidget);
        lbl_1->setObjectName(QStringLiteral("lbl_1"));
        lbl_1->setGeometry(QRect(270, 100, 401, 411));
        lbl_1->setLayoutDirection(Qt::LeftToRight);
        lbl_1->setAlignment(Qt::AlignCenter);
        lbl_2 = new QLabel(centralWidget);
        lbl_2->setObjectName(QStringLiteral("lbl_2"));
        lbl_2->setGeometry(QRect(50, 400, 101, 91));
        lbl_2->setAlignment(Qt::AlignCenter);
        btn_browser = new QPushButton(centralWidget);
        btn_browser->setObjectName(QStringLiteral("btn_browser"));
        btn_browser->setGeometry(QRect(30, 520, 161, 51));
        QtDataGeneratorClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(QtDataGeneratorClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 735, 21));
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
        btn_next->setText(QApplication::translate("QtDataGeneratorClass", "Next image", 0));
        lbl_1->setText(QApplication::translate("QtDataGeneratorClass", "Resting", 0));
        lbl_2->setText(QApplication::translate("QtDataGeneratorClass", "TextLabel", 0));
        btn_browser->setText(QApplication::translate("QtDataGeneratorClass", "Browser", 0));
    } // retranslateUi

};

namespace Ui {
    class QtDataGeneratorClass: public Ui_QtDataGeneratorClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTDATAGENERATOR_H
