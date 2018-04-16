#ifndef CONTROLLER_H
#define CONTROLLER_H
#include "EGM/robot_egm.h"
#include "Threads/vive_thread.h"
#include "Threads/egm_thread.h"
#include "Kinematics/robot.h"
#include <QtCore>
#include <Eigen>


class Controller : public QThread
{

public:
    Controller();
    void run();
    void input_threads(EGM_thread* _egm_thread, Vive_thread* _VR_thread);
    void output_data(Matrix<double, 6, 1> &_joints, Matrix3d &_Rcr, bool &_track_started);

private:
    double p_gain;
    double d_gain;
    double p_gain_angle;
    double xyz_delta;

    double offset_sat;
    double q_sat;

    Matrix<double,6,1> J_min, J_max;

    DH_table IRB120_dh;
    robot IRB120;
    Matrix<double,6,1> J_desired;
    Matrix<double,6,1> J_home;
    Matrix<double,6,1> J_start;
    Matrix<double,6,1> J;
    Matrix3d Rtr_home;
    int controller_id;
    Matrix4d Htr_desired=Matrix4d::Identity();
    Vector3d Xrt_desired;

    Vector3d a1;
    Vector3d a2;
    Vector3d a3;

    Matrix3d Rvr=Matrix3d::Identity();
    Matrix3d Rcv_home;
    Matrix3d RRvr=Matrix3d::Identity();
    Matrix3d Rccnew;

    OFFSET tool_offset;

    JOINTS initial_joints;
    JOINTS start_joints;
    JOINTS joints;

    Vector3d Xcv;
    Vector3d Xcr;
    Vector3d Xcr_start;
    Vector3d Xcr_relative;
    Vector3d Xcr_last;
    Matrix3d Rcr=Matrix3d::Identity();
    Matrix3d initial_R=Matrix3d::Identity();
    Matrix3d last_Rcr=Matrix3d::Identity();
    Matrix3d R_vive_robot=Matrix3d::Identity();

    EGM_thread* egm_thread;
    Vive_thread* VR_thread;
    bool singularity=true;

    bool track_started=false;

    Vector3d Xtr_error;
    Vector3d Xtr_error_last;
    Vector3d Xtr_offset,Xtr_error_d;

    Matrix3d Rtr;

    Matrix<double,3,1>  Xtr_start;
    Matrix<double,3,1>  Xtr;
    Matrix<double,3,1>  Xtr_relative;

    void calculate_offset();
    void singularity_check(bool singularity);
    void set_zero_offset();

};

#endif // CONTROLLER_H
