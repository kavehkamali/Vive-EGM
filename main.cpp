#include "EGM/robot_egm.h"
#include "Threads/vive_thread.h"
#include "Kinematics/robot.h"

#include "mainwindow.h"
#include <QApplication>

void calculate_offset(struct OFFSET (&tool_offset),struct JOINTS (&intial_joints),struct JOINTS (&joints),struct OFFSET (&relative_vive_pose),struct OFFSET (&last_vive_pose));


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}



