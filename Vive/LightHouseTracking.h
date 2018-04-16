#ifndef LIGHTHOUSETRAKING_H
#define LIGHTHOUSETRAKING_H

// OpenVR
#include <openvr.h>
#include "shared\Matrices.h"
#include <Eigen>

struct Data
{
double x=0;
double y=0;
double z=0;

double q1=1;
double q2=0;
double q3=0;
double q4=0;

Eigen::Matrix3d R;

int type=0;
int tracking=0;
};

class LighthouseTracking {
private:

    // Basic stuff
    vr::IVRSystem *m_pHMD = NULL;
    vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
    Matrix4 m_rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];

    // Position and rotation of pose
    vr::HmdVector3_t LighthouseTracking::GetPosition(vr::HmdMatrix34_t matrix);
    vr::HmdQuaternion_t LighthouseTracking::GetRotation(vr::HmdMatrix34_t matrix);
    void LighthouseTracking::GetR(vr::HmdMatrix34_t matrix, Eigen::Matrix3d &R);

    // If false the program will parse tracking data continously and not wait for openvr events
    bool bWaitForEventsBeforeParsing = false;
    std::string vive_message;
    Data data[10];
    int connected_devices=0;

public:
    ~LighthouseTracking();
    LighthouseTracking();
    int connection =-1;




    // Main loop that listens for openvr events and calls process and parse routines, if false the service has quit
    bool RunProcedure();

    // Parse a tracking frame and print its position / rotation
    bool ParseTrackingFrame();

    void get_data(struct Data (& _data)[10]);
    void get_connected_devices(int &_connected_devices);

};






#endif // LIGHTHOUSETRAKING_H
