#include "controller.h"
#include "Kinematics/robot.h"
#include <Eigen>
#include <QDebug>
#include <math.h>
#include <iostream>

Controller::Controller()
{

    p_gain=0.09;
    d_gain=0.00;
    p_gain_angle=0.05;
    xyz_delta=50;

    offset_sat=4;
    q_sat=2;

    J_home<<0, 0, 40, 0, 50, 0;
    Matrix4d start_H=IRB120.FK(J_home);
    Rtr_home=start_H.block(0,0,3,3);

    J_max << 0, 0,  55,  90, 90, 0;
    J_min << 0, 0, -50, -90, 30, 0;

    IRB120_dh.alpha   << 0  , -90 , 0  , -90 , 90, -90 ;
    IRB120_dh.a       << 0  ,  0  , 270,  70 , 0 ,  0  ;
    IRB120_dh.d       << 290,  0  , 0  ,  302, 0 ,  130 ;
    IRB120_dh.theta_0 << 0  , -90 , 0  ,  0  , 0 ,  180;

    IRB120.setDH(IRB120_dh);

    //Calibration XYZ

    a1<<-962.2, -521.9, 383.5;
    a2<< -81.2, -489.1, 386.6;
    a3<<-965.9,  388.2, 397.9;

    Vector3d vx,vy,vz;

    vx=a1-a3;
    vy=a2-a1;
    vz=vx.cross(vy);

    Rvr.col(0)=vx/sqrt(pow(vx(0),2)+pow(vx(1),2)+pow(vx(2),2));
    Rvr.col(1)=vy/sqrt(pow(vy(0),2)+pow(vy(1),2)+pow(vy(2),2));
    Rvr.col(2)=vz/sqrt(pow(vz(0),2)+pow(vz(1),2)+pow(vz(2),2));

    Rcv_home << -0.998827  ,  -0.0484215  ,  -0.00045003,
                -0.0183784  ,  0.387672  ,  -0.921614,
                 0.0448004  ,  -0.920525  ,  -0.388107;

    RRvr <<  0 , -1 , 0 ,
             0 ,  0 , 1 ,
            -1 ,  0 , 0;

    Rccnew=Rcv_home.transpose()*RRvr;
}

void Controller::run()
{
    while(true)
    {
        Sleep(1);

        egm_thread->get_joints(J);
        controller_id=-1;
        for (int kk=0;kk<VR_thread->n_devices;++kk)
        {
            if (VR_thread->vive_Data[kk].type == 2)
            {
                controller_id=kk;
                break;
            }
        }

        if (controller_id==-1)
        {
            track_started=false;
            set_zero_offset();
            egm_thread->set_offset(tool_offset);
            continue;
        }

        Xcv(0) =VR_thread->vive_Data[controller_id].x*1000;
        Xcv(1) =VR_thread->vive_Data[controller_id].y*1000;
        Xcv(2) =VR_thread->vive_Data[controller_id].z*1000;

        Xcr=Rvr.transpose()*Xcv;

        Matrix3d Rcv=Matrix3d::Identity();
        Rcv=VR_thread->vive_Data[controller_id].R;
        Rcr=RRvr.transpose()*Rcv*Rccnew;



        if (VR_thread->vive_Data[controller_id].tracking == 4)
        {
            if (track_started ==false)
            {
                track_started=true;
                J_start=J;
                Xcr_start=Xcr;
            }

            Xcr_relative=Xcr-Xcr_start;

            calculate_offset();
            egm_thread->set_offset(tool_offset);
        }
        else
        {
            track_started=false;
            set_zero_offset();
            egm_thread->set_offset(tool_offset);
        }
    }
}


void Controller::input_threads(EGM_thread* _egm_thread,Vive_thread* _VR_thread)
{
    QMutex mutex;
    mutex.lock();
    egm_thread=_egm_thread;
    VR_thread=_VR_thread;
    mutex.unlock();
}

void Controller::output_data(Matrix<double, 6,1> &_J, Matrix3d &_Rcr, bool &_track_started)
{
    QMutex mutex;
    mutex.lock();
    _J=J;
    _Rcr=Rcr;
    _track_started=track_started;
    mutex.unlock();
}

void Controller::calculate_offset()
{

    Matrix4d H=IRB120.FK(J);
    Rtr=H.block(0,0,3,3);

    Xtr_start= IRB120.get_xyz(J_start);
    Xtr=         IRB120.get_xyz(J);
    Xtr_relative=  Xtr-Xtr_start;



    Xtr_error=Xcr_relative-Xtr_relative;

    Xtr_error_d=(Xtr_error-Xtr_error_last)/0.001;
    Xtr_error_last=Xtr_error;

    Xtr_offset=Xtr_error*p_gain+Xtr_error_d*d_gain;

    if (Xtr(2)<170 && Xtr_offset(2)<0) Xtr_offset(2)=0;


    Matrix<double,3,1> offset;
    offset=Rtr.transpose()*Xtr_offset;



    if (offset(0)> xyz_delta)
        offset(0)= xyz_delta;
    if (offset(0)< -xyz_delta)
        offset(0)= -xyz_delta;

    if (offset(1)> xyz_delta)
        offset(1)= xyz_delta;
    if (offset(1)< -xyz_delta)
        offset(1)= -xyz_delta;

    if (offset(2)> xyz_delta)
        offset(2)= xyz_delta;
    if (offset(2)< -xyz_delta)
        offset(2)= -xyz_delta;

    //********************************************************************
    //********************************************************************


    Matrix3d new_tool;
    new_tool=Rtr_home.transpose();

    Matrix3d Rtr_desired;
    Rtr_desired=Rcr;

    Matrix3d current_R;
    current_R=Rtr*new_tool;

    Matrix3d d_R=current_R.transpose()*Rtr_desired;

    //Rtr.transpose()

    AngleAxisd d_aa(d_R);
    AngleAxisd d_aa_small(d_aa.angle()*p_gain_angle,Rtr.transpose()*d_aa.axis());
    Quaterniond d_q;
    d_q=d_aa_small;



    // SINGULARITY CHECK*********************

    Htr_desired.block(0,0,3,3)=Rtr_desired*new_tool.transpose();

    Xrt_desired = Xcr_relative+Xtr_start;
    Htr_desired.block(0,3,3,1) = Xrt_desired;

    int IK_result = IRB120.IK(Htr_desired,J_desired,0,0,0);

    if (IK_result)
    {
        if (     (J(2)>J_max(2)) && (J_desired(2)>J_max(2))
              || (J(2)<J_min(2)) && (J_desired(2)<J_min(2))
              || (J(3)>J_max(3)) && (J_desired(3)>J_max(3))
              || (J(3)<J_min(3)) && (J_desired(3)<J_min(3))
              || (J(4)>J_max(4)) && (J_desired(4)>J_max(4))
              || (J(4)<J_min(4)) && (J_desired(4)<J_min(4))
              )
        {
            qDebug() << "OUT OF RANGE!!!";
            set_zero_offset();

        }
        else if (     (J_desired(2)>J_max(2))
                   || (J_desired(2)<J_min(2))
                   || (J_desired(3)>J_max(3))
                   || (J_desired(3)<J_min(3))
                   || (J_desired(4)>J_max(4))
                   || (J_desired(4)<J_min(4))
                   )
        {


        }
        else
        {


            if (offset(0)>offset_sat)
                tool_offset.x=offset_sat;
            else if (offset(0)<-offset_sat)
                tool_offset.x=-offset_sat;
            else
                tool_offset.x=offset(0);


            if (offset(1)>offset_sat)
                tool_offset.y=offset_sat;
            else if (offset(1)<-offset_sat)
                tool_offset.y=-offset_sat;
            else
                tool_offset.y=offset(1);


            if (offset(2)>offset_sat)
                tool_offset.z=offset_sat;
            else if (offset(2)<-offset_sat)
                tool_offset.z=-offset_sat;
            else
                tool_offset.z=offset(2);


            if (d_q.w()>q_sat)
                tool_offset.q1=q_sat;
            else if (d_q.w()<-q_sat)
                tool_offset.q1=-q_sat;
            else
                tool_offset.q1=d_q.w();


            if (d_q.x()>q_sat)
                tool_offset.q2=q_sat;
            else if (d_q.x()<-q_sat)
                tool_offset.q2=-q_sat;
            else
                tool_offset.q2=d_q.x();

            if (d_q.y()>q_sat)
                tool_offset.q3=q_sat;
            else if (d_q.y()<-q_sat)
                tool_offset.q3=-q_sat;
            else
                tool_offset.q3=d_q.y();

            if (d_q.z()>q_sat)
                tool_offset.q4=q_sat;
            else if (d_q.z()<-q_sat)
                tool_offset.q4=-q_sat;
            else
                tool_offset.q4=d_q.z();

        }
    }
    else
    {
        set_zero_offset();
    }

}

void Controller::set_zero_offset()
{
    tool_offset.x=0;
    tool_offset.y=0;
    tool_offset.z=0;

    tool_offset.q1=1;
    tool_offset.q2=0;
    tool_offset.q3=0;
    tool_offset.q4=0;
}
