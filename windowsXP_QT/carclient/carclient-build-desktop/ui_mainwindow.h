/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Mar 3 10:42:16 2015
**      by: Qt User Interface Compiler version 4.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QPushButton *connect;
    QPushButton *disconnect;
    QLineEdit *ipserveradd;
    QLineEdit *port;
    QLabel *statuslabel;
    QPushButton *send;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(253, 247);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        connect = new QPushButton(centralWidget);
        connect->setObjectName(QString::fromUtf8("connect"));
        connect->setGeometry(QRect(20, 90, 72, 23));
        disconnect = new QPushButton(centralWidget);
        disconnect->setObjectName(QString::fromUtf8("disconnect"));
        disconnect->setGeometry(QRect(160, 90, 72, 23));
        ipserveradd = new QLineEdit(centralWidget);
        ipserveradd->setObjectName(QString::fromUtf8("ipserveradd"));
        ipserveradd->setGeometry(QRect(20, 20, 131, 20));
        port = new QLineEdit(centralWidget);
        port->setObjectName(QString::fromUtf8("port"));
        port->setGeometry(QRect(170, 20, 61, 20));
        statuslabel = new QLabel(centralWidget);
        statuslabel->setObjectName(QString::fromUtf8("statuslabel"));
        statuslabel->setGeometry(QRect(20, 50, 211, 16));
        send = new QPushButton(centralWidget);
        send->setObjectName(QString::fromUtf8("send"));
        send->setGeometry(QRect(20, 130, 75, 23));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 253, 19));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        connect->setText(QApplication::translate("MainWindow", "connect", 0, QApplication::UnicodeUTF8));
        disconnect->setText(QApplication::translate("MainWindow", "disconnect", 0, QApplication::UnicodeUTF8));
        ipserveradd->setText(QApplication::translate("MainWindow", "192.168.7.1", 0, QApplication::UnicodeUTF8));
        port->setText(QApplication::translate("MainWindow", "8980", 0, QApplication::UnicodeUTF8));
        statuslabel->setText(QApplication::translate("MainWindow", "TextLabel", 0, QApplication::UnicodeUTF8));
        send->setText(QApplication::translate("MainWindow", "send data", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
