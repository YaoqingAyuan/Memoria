/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *label;
    QLineEdit *videoEdit;
    QPushButton *videoBtn;
    QLabel *label_2;
    QPushButton *audioBtn;
    QLineEdit *audioEdit;
    QLineEdit *outputEdit;
    QPushButton *outputBtn;
    QLabel *label_3;
    QPushButton *pushButton;
    QProgressBar *progressBar;
    QPushButton *titlefolderBtn;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(140, 150, 81, 31));
        QFont font;
        font.setPointSize(10);
        label->setFont(font);
        videoEdit = new QLineEdit(centralwidget);
        videoEdit->setObjectName("videoEdit");
        videoEdit->setGeometry(QRect(220, 150, 361, 31));
        videoBtn = new QPushButton(centralwidget);
        videoBtn->setObjectName("videoBtn");
        videoBtn->setGeometry(QRect(580, 150, 93, 31));
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(140, 210, 81, 31));
        label_2->setFont(font);
        audioBtn = new QPushButton(centralwidget);
        audioBtn->setObjectName("audioBtn");
        audioBtn->setGeometry(QRect(580, 210, 93, 31));
        audioEdit = new QLineEdit(centralwidget);
        audioEdit->setObjectName("audioEdit");
        audioEdit->setGeometry(QRect(220, 210, 361, 31));
        outputEdit = new QLineEdit(centralwidget);
        outputEdit->setObjectName("outputEdit");
        outputEdit->setGeometry(QRect(220, 270, 361, 31));
        outputBtn = new QPushButton(centralwidget);
        outputBtn->setObjectName("outputBtn");
        outputBtn->setGeometry(QRect(580, 270, 93, 31));
        label_3 = new QLabel(centralwidget);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(140, 270, 81, 31));
        label_3->setFont(font);
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(280, 340, 251, 51));
        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName("progressBar");
        progressBar->setGeometry(QRect(180, 420, 471, 23));
        progressBar->setValue(0);
        titlefolderBtn = new QPushButton(centralwidget);
        titlefolderBtn->setObjectName("titlefolderBtn");
        titlefolderBtn->setGeometry(QRect(80, 100, 191, 28));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 25));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\350\247\206\351\242\221\346\226\207\344\273\266\357\274\232", nullptr));
        videoBtn->setText(QCoreApplication::translate("MainWindow", "\346\265\217\350\247\210...", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\351\237\263\351\242\221\346\226\207\344\273\266\357\274\232", nullptr));
        audioBtn->setText(QCoreApplication::translate("MainWindow", "\346\265\217\350\247\210...", nullptr));
        outputBtn->setText(QCoreApplication::translate("MainWindow", "\346\265\217\350\247\210...", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\350\276\223\345\207\272\350\267\257\345\276\204\357\274\232", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "\345\274\200\345\247\213\345\220\210\345\271\266", nullptr));
        titlefolderBtn->setText(QCoreApplication::translate("MainWindow", "\346\265\217\350\247\210\346\240\207\351\242\230\346\226\207\344\273\266\345\244\271 ...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
