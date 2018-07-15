# Qt-C++ code for real-time control of the ABB IRB 120 using EGM (Pose Mode) and  HTC Vive controller 

<p align="center">
    <img src="https://github.com/kavehkamali/JointMode-EGM-Vive-Python/blob/master/demo.gif" width="500">
</p>

This code uses OpenVR and EGM for real-time remote control of an ABB 120 robot. The package pyopenvr is used to receive position and orientation of an HTC Vive controller. Then a PID controller sends the position and orientation to the ABB robot. We used a python wrapper of EGM in the Pose Mode. The python wrapper is made by boost-python.

## Prerequisite steps:

Note: the code is tested in Windows OS with VS 2015 64x

1 - Install SteamVR

2 - Compile and install Eigen at "C:\Eigen"

3 - Compile and install OpenVR at "C:\openvr"

4 - Compile and install google protobuf at "C:\protobuf-3.4.1"

3 - Edit the "udp_connection_name" in the RAPID module "EGM_connection.mod".

4 - Copy the RAPID module "EGM_connection.mod" to the ABB robot controller.

## To run the code:

1- Run "EGM_connection.mod" on the ABB robot controler

2- Run SteamVR

3- Run the Qt code 

