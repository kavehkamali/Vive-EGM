#ifndef EGM_THREAD_H
#define EGM_THREAD_H
#include "EGM/robot_egm.h"
#include <iostream>
#include <QProcess>
#include "Windows.h"
#include <Eigen>

#include "Vive/LighthouseTracking.h"

// windows keyboard input
#include <conio.h>

#include <QtCore>

class EGM_thread  : public QThread
{
public:
    EGM_thread();
    Robot_EGM new_egm;
    void run();
    void get_joints(Eigen::Matrix<double,6,1> &_J);
    void set_offset(OFFSET _Offset);
    bool isconnected();
    bool connected=false;
private:
    OFFSET Offset;
    JOINTS Joints;

};

#endif // EGM_THREAD_H
