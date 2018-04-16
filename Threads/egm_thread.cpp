#include "egm_thread.h"
#include "EGM/robot_egm.h"
#include <Eigen>

using namespace Eigen;

EGM_thread::EGM_thread()
{

}

void EGM_thread::run()
{
    if (!connected)
    {
        if (new_egm.connect())
        {
            if (new_egm.loop())
            {
            connected=true;
            }
        }
    }
    else
    {
        connected=false;
    }

    while(connected)
    {
    new_egm.getJoints(Joints);
    new_egm.setOffset(Offset);
    connected=new_egm.loop();
    }
}

void EGM_thread::get_joints(Matrix<double,6,1> &_J)
{
    QMutex mutex;
    mutex.lock();
    _J<<Joints.j1 ,Joints.j2 ,Joints.j3 ,Joints.j4 ,Joints.j5 ,Joints.j6;
    mutex.unlock();
}

void EGM_thread::set_offset(OFFSET _Offset)
{
    QMutex mutex;
    mutex.lock();
    Offset=_Offset;
    mutex.unlock();
}

bool EGM_thread::isconnected()
{
    bool result=false;
    QMutex mutex;
    mutex.lock();
    result=connected;
    mutex.unlock();
    return result;
}
