
#include "LighthouseTracking.h"
#include <Eigen>
#include <QDebug>
int devicenum=0;
// Destructor
LighthouseTracking::~LighthouseTracking() {
	if (m_pHMD != NULL)
	{
		vr::VR_Shutdown();
		m_pHMD = NULL;
	}
}

// Constructor
LighthouseTracking::LighthouseTracking() {
	vr::EVRInitError eError = vr::VRInitError_None;
	m_pHMD = vr::VR_Init(&eError, vr::VRApplication_Background);

	if (eError != vr::VRInitError_None)
	{
		m_pHMD = NULL;
        //char buf[1024];
        //sprintf_s(buf, sizeof(buf), "Unable to init VR runtime: %s", vr::VR_GetVRInitErrorAsEnglishDescription(eError));
        //printf_s(buf);
        connection=0;
        //exit(EXIT_FAILURE);

	}
    connection=1;
}

/*
* Loop-listen for events then parses them (e.g. prints the to user)
* Returns true if success or false if openvr has quit
*/
bool LighthouseTracking::RunProcedure() {
    return ParseTrackingFrame();
}

// Get the quaternion representing the rotation
vr::HmdQuaternion_t LighthouseTracking::GetRotation(vr::HmdMatrix34_t matrix) {
	vr::HmdQuaternion_t q;

	q.w = sqrt(fmax(0, 1 + matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2])) / 2;
	q.x = sqrt(fmax(0, 1 + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2])) / 2;
	q.y = sqrt(fmax(0, 1 - matrix.m[0][0] + matrix.m[1][1] - matrix.m[2][2])) / 2;
	q.z = sqrt(fmax(0, 1 - matrix.m[0][0] - matrix.m[1][1] + matrix.m[2][2])) / 2;
	q.x = copysign(q.x, matrix.m[2][1] - matrix.m[1][2]);
	q.y = copysign(q.y, matrix.m[0][2] - matrix.m[2][0]);
	q.z = copysign(q.z, matrix.m[1][0] - matrix.m[0][1]);
	return q;
}

void LighthouseTracking::GetR(vr::HmdMatrix34_t matrix, Eigen::Matrix3d &R) {
    vr::HmdQuaternion_t q;

R <<    matrix.m[0][0], matrix.m[0][1], matrix.m[0][2],
        matrix.m[1][0], matrix.m[1][1], matrix.m[1][2],
        matrix.m[2][0], matrix.m[2][1], matrix.m[2][2];
}


// Get the vector representing the position
vr::HmdVector3_t LighthouseTracking::GetPosition(vr::HmdMatrix34_t matrix) {
	vr::HmdVector3_t vector;

	vector.v[0] = matrix.m[0][3];
	vector.v[1] = matrix.m[1][3];
	vector.v[2] = matrix.m[2][3];

	return vector;
}

/*
* Parse a Frame with data from the tracking system
*
* Handy reference:
* https://github.com/TomorrowTodayLabs/NewtonVR/blob/master/Assets/SteamVR/Scripts/SteamVR_Utils.cs
*
* Also:
* Open VR Convention (same as OpenGL)
* right-handed system
* +y is up
* +x is to the right
* -z is going away from you
* http://www.3dgep.com/understanding-the-view-matrix/
*
*/
bool LighthouseTracking::ParseTrackingFrame() {

	// Process SteamVR device states

	for (vr::TrackedDeviceIndex_t unDevice = 0; unDevice < vr::k_unMaxTrackedDeviceCount; unDevice++)
	{
        if (m_pHMD==NULL)
            return false;
		// if not connected just skip the rest of the routine
		if (!m_pHMD->IsTrackedDeviceConnected(unDevice))
			continue;
        else
        {
            devicenum++;
            //std::cout <<std::endl<< "Device number :" <<devicenum<<std::endl;
        }
		vr::TrackedDevicePose_t trackedDevicePose;
		vr::TrackedDevicePose_t *devicePose = &trackedDevicePose;

		vr::VRControllerState_t controllerState;
        vr::VRControllerState_t *ontrollerState_ptr = &controllerState;

		vr::HmdVector3_t position;
        vr::HmdQuaternion_t quaternion;
        Eigen::Matrix3d R;

		if (vr::VRSystem()->IsInputFocusCapturedByAnotherProcess()) {
            //char buf[1024];
            //sprintf_s(buf, sizeof(buf), "\nInput Focus by Another Process\n");
            //printf_s(buf);
            data[unDevice].x=0;
            data[unDevice].y=0;
            data[unDevice].z=0;
            data[unDevice].type=0;
            data[unDevice].tracking=0;

        }


		bool bPoseValid = trackedDevicePose.bPoseIsValid;
		vr::HmdVector3_t vVel;
		vr::HmdVector3_t vAngVel;
		vr::ETrackingResult eTrackingResult;

		// Get what type of device it is and work with its data
        vr::ETrackedDeviceClass trackedDeviceClass = vr::VRSystem()->GetTrackedDeviceClass(unDevice);
		switch (trackedDeviceClass) {

		case vr::ETrackedDeviceClass::TrackedDeviceClass_HMD:
            //std::cout<< "HMD device" <<std::endl;
            data[unDevice].x=0;
            data[unDevice].y=0;
            data[unDevice].z=0;
            data[unDevice].type=1;
            data[unDevice].tracking=0;
            break;

		case vr::ETrackedDeviceClass::TrackedDeviceClass_Controller:
            //std::cout<< "Controller device" <<std::endl;
			// Simliar to the HMD case block above, please adapt as you like
			// to get away with code duplication and general confusion

			vr::VRSystem()->GetControllerStateWithPose(vr::TrackingUniverseStanding, unDevice, &controllerState, sizeof(controllerState), &trackedDevicePose);

			position = GetPosition(devicePose->mDeviceToAbsoluteTracking);
			quaternion = GetRotation(devicePose->mDeviceToAbsoluteTracking);
            GetR(devicePose->mDeviceToAbsoluteTracking,R);




			vVel = trackedDevicePose.vVelocity;
			vAngVel = trackedDevicePose.vAngularVelocity;
			eTrackingResult = trackedDevicePose.eTrackingResult;
			bPoseValid = trackedDevicePose.bPoseIsValid;

			switch (vr::VRSystem()->GetControllerRoleForTrackedDeviceIndex(unDevice)) {
			case vr::TrackedControllerRole_Invalid:
                //std::cout<< "Invalid Hand" <<std::endl;
                data[unDevice].x=0;
                data[unDevice].y=0;
                data[unDevice].z=0;
                data[unDevice].q1=1;
                data[unDevice].q2=0;
                data[unDevice].q3=0;
                data[unDevice].q4=0;
                data[unDevice].type=0;
                data[unDevice].tracking=0;
                break;

			//
			case vr::TrackedControllerRole_LeftHand:

                char buf[1024];

                //sprintf_s(buf, sizeof(buf), "\nLeft Controller\nx: %.2f y: %.2f z: %.2f\n", position.v[0], position.v[1], position.v[2]);
                //printf_s(buf);

                //sprintf_s(buf, sizeof(buf), "qw: %.2f qx: %.2f qy: %.2f qz: %.2f\n", quaternion.w, quaternion.x, quaternion.y, quaternion.z);
                //printf_s(buf);

                data[unDevice].x=double(-position.v[0]);
                data[unDevice].y=double(position.v[2]);
                data[unDevice].z=double(position.v[1]);
                data[unDevice].q1=double(quaternion.w);
                data[unDevice].q2=double(quaternion.x);
                data[unDevice].q3=double(quaternion.y);
                data[unDevice].q4=double(quaternion.z);
                data[unDevice].R=R;
                data[unDevice].type=3;

				switch (eTrackingResult) {
				case vr::ETrackingResult::TrackingResult_Uninitialized:
                    //sprintf_s(buf, sizeof(buf), "Invalid tracking result\n");
                    //printf_s(buf);


                    data[unDevice].tracking=0;
					break;
				case vr::ETrackingResult::TrackingResult_Calibrating_InProgress:
                    //sprintf_s(buf, sizeof(buf), "Calibrating in progress\n");
                    //printf_s(buf);
                    data[unDevice].tracking=0;
					break;
				case vr::ETrackingResult::TrackingResult_Calibrating_OutOfRange:
                    //sprintf_s(buf, sizeof(buf), "Calibrating Out of range\n");
                    //printf_s(buf);
                    data[unDevice].tracking=0;
					break;
				case vr::ETrackingResult::TrackingResult_Running_OK:
                    //sprintf_s(buf, sizeof(buf), "Running OK\n");
                    //printf_s(buf);
                    data[unDevice].tracking=controllerState.ulButtonPressed;
					break;
				case vr::ETrackingResult::TrackingResult_Running_OutOfRange:
                    //sprintf_s(buf, sizeof(buf), "WARNING: Running Out of Range\n");
                    //printf_s(buf);
                    data[unDevice].tracking=0;

					break;
				default:
                    //sprintf_s(buf, sizeof(buf), "Default\n");
                    //printf_s(buf);
                    data[unDevice].tracking=0;
					break;
				}

				if (bPoseValid)
                    //sprintf_s(buf, sizeof(buf), "Valid pose\n");
                    data[unDevice].tracking=controllerState.ulButtonPressed;

				else
                    //sprintf_s(buf, sizeof(buf), "Invalid pose\n");
                data[unDevice].tracking=0;
                //printf_s(buf);
				break;

			case vr::TrackedControllerRole_RightHand:
				// incomplete code, look at left hand for reference

                //char buf2[1024];

                //sprintf_s(buf2, sizeof(buf2), "\nRight Controller\nx: %.2f y: %.2f z: %.2f\n", position.v[0], position.v[1], position.v[2]);
                //printf_s(buf2);

                //sprintf_s(buf2, sizeof(buf2), "qw: %.2f qx: %.2f qy: %.2f qz: %.2f\n", quaternion.w, quaternion.x, quaternion.y, quaternion.z);
                //printf_s(buf2);

                data[unDevice].x=double(-position.v[0]);
                data[unDevice].y=double(position.v[2]);
                data[unDevice].z=double(position.v[1]);
                data[unDevice].q1=double(quaternion.w);
                data[unDevice].q2=double(quaternion.x);
                data[unDevice].q3=double(quaternion.y);
                data[unDevice].q4=double(quaternion.z);
                data[unDevice].R=R;
                data[unDevice].type=2;

                switch (eTrackingResult) {
                case vr::ETrackingResult::TrackingResult_Uninitialized:
                    //sprintf_s(buf2, sizeof(buf2), "Invalid tracking result\n");
                    //printf_s(buf2);
                    data[unDevice].tracking=0;
                    break;
                case vr::ETrackingResult::TrackingResult_Calibrating_InProgress:
                    //sprintf_s(buf2, sizeof(buf2), "Calibrating in progress\n");
                    //printf_s(buf2);
                    data[unDevice].tracking=0;
                    break;
                case vr::ETrackingResult::TrackingResult_Calibrating_OutOfRange:
                    //sprintf_s(buf2, sizeof(buf2), "Calibrating Out of range\n");
                    //printf_s(buf2);
                    data[unDevice].tracking=0;
                    break;
                case vr::ETrackingResult::TrackingResult_Running_OK:
                    //sprintf_s(buf2, sizeof(buf2), "Running OK\n");
                    //printf_s(buf2);
                    //data[unDevice].tracking=1;

                    data[unDevice].tracking=controllerState.ulButtonPressed;
                    /*
                    if (vr::ButtonMaskFromId(vr::k_EButton_Axis0)==controllerState.ulButtonPressed)
                        data[unDevice].tracking=2;
                        */

                    break;
                case vr::ETrackingResult::TrackingResult_Running_OutOfRange:
                    //sprintf_s(buf2, sizeof(buf2), "WARNING: Running Out of Range\n");
                    //printf_s(buf2);
                    data[unDevice].tracking=0;
                    break;
                default:
                    //sprintf_s(buf2, sizeof(buf2), "Default\n");
                    //printf_s(buf2);
                    data[unDevice].tracking=0;
                    break;
                }

                if (bPoseValid)
                    //sprintf_s(buf2, sizeof(buf2), "Valid pose\n");
                    data[unDevice].tracking=controllerState.ulButtonPressed;
                else
                    //sprintf_s(buf2, sizeof(buf2), "Invalid pose\n");
                    data[unDevice].tracking=0;
                    //printf_s(buf2);
                break;

			case vr::TrackedDeviceClass_TrackingReference:
                //std::cout <<std::endl<<"Tracking reference"<<std::endl;
				// incomplete code, only here for switch reference
                //sprintf_s(buf, sizeof(buf), "Camera / Base Station");
                //printf_s(buf);
                data[unDevice].x=double(-position.v[0]);
                data[unDevice].y=double(position.v[2]);
                data[unDevice].z=double(position.v[1]);
                data[unDevice].q1=double(quaternion.w);
                data[unDevice].q2=double(quaternion.x);
                data[unDevice].q3=double(quaternion.y);
                data[unDevice].q4=double(quaternion.z);
                data[unDevice].R=R;
                data[unDevice].type=0;
                data[unDevice].tracking=0;
                break;
			}

			break;
		}
	}

//std::cout <<std::endl<<"Number of connected devices:  "<<devicenum<<std::endl;
    connected_devices=devicenum;
devicenum=0;
return true;
}

void LighthouseTracking::get_data(struct Data (& _data)[10])
{
    for (int i=0;i<connected_devices;++i)
    _data[i]=data[i];
}
void LighthouseTracking::get_connected_devices(int &_connected_devices)
{
    _connected_devices=connected_devices;
}
