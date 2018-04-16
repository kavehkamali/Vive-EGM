/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionLoad_calibration;
    QAction *actionSave_calibration;
    QWidget *centralWidget;
    QPushButton *EGM_Button;
    QPushButton *Vive_Button;
    QPushButton *Controller_Button;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *joints_label;
    QLabel *pose_label;
    QLabel *button_label;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(851, 349);
        MainWindow->setContextMenuPolicy(Qt::DefaultContextMenu);
        actionLoad_calibration = new QAction(MainWindow);
        actionLoad_calibration->setObjectName(QStringLiteral("actionLoad_calibration"));
        actionSave_calibration = new QAction(MainWindow);
        actionSave_calibration->setObjectName(QStringLiteral("actionSave_calibration"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        EGM_Button = new QPushButton(centralWidget);
        EGM_Button->setObjectName(QStringLiteral("EGM_Button"));
        EGM_Button->setGeometry(QRect(50, 30, 131, 51));
        Vive_Button = new QPushButton(centralWidget);
        Vive_Button->setObjectName(QStringLiteral("Vive_Button"));
        Vive_Button->setGeometry(QRect(340, 30, 141, 51));
        Controller_Button = new QPushButton(centralWidget);
        Controller_Button->setObjectName(QStringLiteral("Controller_Button"));
        Controller_Button->setGeometry(QRect(630, 30, 141, 51));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(40, 120, 141, 31));
        QFont font;
        font.setPointSize(14);
        font.setBold(true);
        font.setWeight(75);
        label->setFont(font);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(40, 170, 171, 31));
        label_2->setFont(font);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(40, 220, 191, 31));
        label_3->setFont(font);
        joints_label = new QLabel(centralWidget);
        joints_label->setObjectName(QStringLiteral("joints_label"));
        joints_label->setGeometry(QRect(270, 120, 561, 31));
        QFont font1;
        font1.setPointSize(12);
        font1.setBold(false);
        font1.setWeight(50);
        joints_label->setFont(font1);
        pose_label = new QLabel(centralWidget);
        pose_label->setObjectName(QStringLiteral("pose_label"));
        pose_label->setGeometry(QRect(270, 170, 561, 31));
        pose_label->setFont(font1);
        button_label = new QLabel(centralWidget);
        button_label->setObjectName(QStringLiteral("button_label"));
        button_label->setGeometry(QRect(270, 220, 141, 31));
        QFont font2;
        font2.setPointSize(12);
        font2.setBold(true);
        font2.setWeight(75);
        button_label->setFont(font2);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 851, 21));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionLoad_calibration);
        menuFile->addAction(actionSave_calibration);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "EGM Vive", Q_NULLPTR));
        actionLoad_calibration->setText(QApplication::translate("MainWindow", "Load calibration", Q_NULLPTR));
        actionSave_calibration->setText(QApplication::translate("MainWindow", "Save calibration", Q_NULLPTR));
        EGM_Button->setText(QApplication::translate("MainWindow", "EGM", Q_NULLPTR));
        Vive_Button->setText(QApplication::translate("MainWindow", "Vive", Q_NULLPTR));
        Controller_Button->setText(QApplication::translate("MainWindow", "Controller", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Joint Angles:", Q_NULLPTR));
        label_2->setText(QApplication::translate("MainWindow", "Controller Pose:", Q_NULLPTR));
        label_3->setText(QApplication::translate("MainWindow", "Side button status:", Q_NULLPTR));
        joints_label->setText(QApplication::translate("MainWindow", "...", Q_NULLPTR));
        pose_label->setText(QApplication::translate("MainWindow", "...", Q_NULLPTR));
        button_label->setText(QApplication::translate("MainWindow", "...", Q_NULLPTR));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", Q_NULLPTR));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
