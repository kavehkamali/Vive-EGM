#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer *ui_timer=new QTimer(this);
    connect(ui_timer,SIGNAL(timeout()),this,SLOT(ui_refresh()));
    ui_timer->start(100);

    connect(ui->Vive_Button, SIGNAL(released()),this,SLOT(connect_vive()));
    connect(ui->EGM_Button, SIGNAL(released()),this,SLOT(connect_EGM()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ui_refresh()
{
    //***************************************************************
    //***************************************************************
    if (egm_thread.isconnected()&&VR_thread.isconnected())
    {
        if (!controller_started)
        {
            controller_thread.input_threads(&egm_thread,&VR_thread);
            controller_thread.start();
            ui->Controller_Button->setStyleSheet("background-color: green");
            controller_started=true;
        }
        controller_thread.output_data(joints, abs_R, track_started);
    }
    else
    {
        ui->Controller_Button->setStyleSheet("background-color: red");
        controller_started=false;
    }

    //********************************************************************
    //********************************************************************

    QString joints_str="...";
    QString pose_str="...";

    if (egm_thread.isconnected())
    {
        ui->EGM_Button->setStyleSheet("background-color: green");
        EGM_started=true;
        joints_str =  QString::number( joints(0) ) + ", " +
                              QString::number( joints(1) ) + ", " +
                              QString::number( joints(2) ) + ", " +
                              QString::number( joints(3) ) + ", " +
                              QString::number( joints(4)) + ", " +
                              QString::number( joints(5) ) ;
    }
    else
    {
        ui->EGM_Button->setStyleSheet("background-color: red");
        EGM_started=false;
        joints_str="...";
    }

    if (VR_thread.isconnected())
    {
        ui->Vive_Button->setStyleSheet("background-color: green");
        Vive_started=true;
        pose_str =    QString::number( abs_R(0,0) ) + ", " +
                              QString::number( abs_R(1,1) ) + ", " +
                              QString::number( abs_R(2,2) );
    }
    else
    {
        //VR_thread.exit();
        ui->Vive_Button->setStyleSheet("background-color: red");
        Vive_started=false;
        pose_str="...";
    }
    ui->joints_label->setText(joints_str);
    ui->pose_label->setText(pose_str);
    if (track_started)
        ui->button_label->setText("<font color='green'>Pressed</font>");
    else
        ui->button_label->setText("<font color='red'>Released</font>");

    //*********************************************************************
    //*********************************************************************
}

void MainWindow::connect_vive()
{
    if (!Vive_started)
    {
        VR_thread.start();
        //Sleep(2000);
        if (VR_thread.isconnected())
        {
            ui->Vive_Button->setStyleSheet("background-color: green");
            Vive_started=true;
        }
        else
        {
            VR_thread.exit();
            ui->Vive_Button->setStyleSheet("background-color: red");
            Vive_started=false;
        }
    }
    else
    {
        VR_thread.exit();
        ui->Vive_Button->setStyleSheet("background-color: red");
        Vive_started=false;
    }
}


void MainWindow::connect_EGM()
{
    if (!EGM_started)
    {
        egm_thread.start();
        Sleep(1000);
        if (egm_thread.isconnected())
        {
            ui->EGM_Button->setStyleSheet("background-color: green");
            EGM_started=true;
        }
        else
        {
            ui->EGM_Button->setStyleSheet("background-color: red");
            EGM_started=false;
        }
    }
    else
    {
        ui->EGM_Button->setStyleSheet("background-color: red");
        EGM_started=false;
    }
}

