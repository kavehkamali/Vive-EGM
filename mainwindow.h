#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "EGM/robot_egm.h"
#include "Threads/vive_thread.h"
#include "Kinematics/robot.h"
#include "Controller/controller.h"
#include "Threads/egm_thread.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    EGM_thread egm_thread;
    Vive_thread VR_thread;
    Controller controller_thread;
    bool Vive_started=false;
    bool EGM_started=false;
    bool controller_started=false;
    bool track_started=false;
    Matrix<double, 6, 1> joints;
    Matrix3d abs_R;

private slots:
    void connect_vive();
    void connect_EGM();
    void ui_refresh();
};

#endif // MAINWINDOW_H
