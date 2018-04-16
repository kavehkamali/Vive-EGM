#ifndef VIVE_THREAD_H
#define VIVE_THREAD_H

#include <iostream>
#include <QProcess>
#include "Windows.h"

#include "Vive/LighthouseTracking.h"

// windows keyboard input
#include <conio.h>

#include <QtCore>

class Vive_thread : public QThread
{
public:
    Vive_thread();
    void run();
    Data vive_Data[10];
    int n_devices;
    //std::string vive_message;
    bool isconnected();
private:
    int connection=-1;
};

#endif // VIVE_THREAD_H
