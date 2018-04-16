#include "vive_thread.h"
#include <QDebug>


Vive_thread::Vive_thread()
{
    /*
    QMutex mutex;
    mutex.lock();
    for(int i=1;i<10;i++)
    {
        vive_Data[i].x=0;
        vive_Data[i].y=0;
        vive_Data[i].z=0;
        vive_Data[i].q1=0;
        vive_Data[i].q2=0;
        vive_Data[i].q3=0;
        vive_Data[i].q4=0;
        vive_Data[i].type=0;
        vive_Data[i].tracking=0;
    }
    n_devices=0;
    mutex.unlock();
    */

}

void Vive_thread::run()
{
    // If false we'll parse tracking data continously, if true we parse when an openvr event fires
    bool bAcquireTrackingDataByWaitingForVREvents = false;

    // Create a new LighthouseTracking instance and parse as needed
    LighthouseTracking *lighthouseTracking = new LighthouseTracking();


    if (lighthouseTracking->connection==1)
    {
        if (lighthouseTracking) {

            //char buf[1024];
            //sprintf_s(buf, sizeof(buf), "Press 'q' to quit. Starting capture of tracking data...\n");
            //printf_s(buf);
            Sleep(2000);
            while (lighthouseTracking->RunProcedure()) {
                connection=1;
                // Windows quit routine - adapt as you need
                //if (_kbhit()) {
                //   char ch = _getch();
                //    if ('q' == ch) {

                //        char buf[1024];
                //        sprintf_s(buf, sizeof(buf), "User pressed 'q' - exiting...");
                //        printf_s(buf);

                //        break;
                //    }
                //}

                // a delay to not overheat your computer... :)
                Sleep(1);
                //std::system("cls");
                QMutex mutex;
                mutex.lock();

                lighthouseTracking->get_data(vive_Data);
                lighthouseTracking->get_connected_devices(n_devices);
                mutex.unlock();
            }
        }

        connection=0;
        delete lighthouseTracking;
    }

}

bool Vive_thread::isconnected()
{
    bool result=false;
    QMutex mutex;
    mutex.lock();
    if (connection==1)
        result=true;
    else
        result=false;
    mutex.unlock();
    return result;
}
