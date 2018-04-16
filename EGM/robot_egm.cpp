#include "robot_egm.h"


Robot_EGM::Robot_EGM()
{
}

bool Robot_EGM::connect()
{
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
    {
        fprintf(stderr, "Could not open Windows connection.\n");
        return false;
    }

    // create socket to listen on
    sockfd = ::socket(AF_INET,SOCK_DGRAM,0);

    memset(&serverAddr, sizeof(serverAddr), 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(portNumber);

    // listen on all interfaces
    bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    return true;
}

bool Robot_EGM::loop()
{
    string messageBuffer;
    // receive and display message from robot
    len = sizeof(clientAddr);
    n = recvfrom(sockfd, protoMessage, 1400, 0, (struct sockaddr *)&clientAddr, &len);

    if (n < 0)
    {
        printf("Error receive message\n");
        return false;
    }
    else
    {
        // deserialize inbound message
        EgmRobot *pRobotMessage = new EgmRobot();
        pRobotMessage->ParseFromArray(protoMessage, n);
        int message_result=DisplayRobotMessage(pRobotMessage);
        delete pRobotMessage;
        if (message_result==0) return 1;

        // create and send a sensor message
        EgmSensor *pSensorMessage = new EgmSensor();
        CreateSensorMessage(pSensorMessage);
        pSensorMessage->SerializeToString(&messageBuffer);

        // send a message to the robot
        n = sendto(sockfd, messageBuffer.c_str(), messageBuffer.length(), 0, (struct sockaddr *)&clientAddr, sizeof(clientAddr));
        if (n < 0)
        {
            printf("Error send message\n");
            return false;
        }
        delete pSensorMessage;
        return true;
    }
}

//////////////////////////////////////////////////////////////////////////
// Create a simple sensor message
void Robot_EGM::CreateSensorMessage(EgmSensor* pSensorMessage)
{
    EgmHeader* header = new EgmHeader();
    header->set_mtype(EgmHeader_MessageType_MSGTYPE_CORRECTION);
    header->set_seqno(sequenceNumber++);
    header->set_tm(GetTickCount());

    pSensorMessage->set_allocated_header(header);

    EgmCartesian *pc = new EgmCartesian();
    pc->set_x(offset_x);
    pc->set_y(offset_y);
    pc->set_z(offset_z);

    EgmQuaternion *pq = new EgmQuaternion();
    pq->set_u0(q1);
    pq->set_u1(q2);
    pq->set_u2(q3);
    pq->set_u3(q4);

    EgmPose *pcartesian = new EgmPose();
    pcartesian->set_allocated_orient(pq);
    pcartesian->set_allocated_pos(pc);

    EgmPlanned *planned = new EgmPlanned();
    planned->set_allocated_cartesian(pcartesian);

    pSensorMessage->set_allocated_planned(planned);
}

//////////////////////////////////////////////////////////////////////////
// Display inbound robot message
int Robot_EGM::DisplayRobotMessage(EgmRobot *pRobotMessage)
{
    if (pRobotMessage->has_header() && pRobotMessage->header().has_seqno() && pRobotMessage->header().has_tm() && pRobotMessage->header().has_mtype())
    {
        //printf("SeqNo=%d Tm=%u Type=%d\n", pRobotMessage->header().seqno(), pRobotMessage->header().tm(), pRobotMessage->header().mtype());
        connection_seqno=pRobotMessage->header().seqno();
        robot_feedback=pRobotMessage->feedback();
        egmjoints=robot_feedback.joints();
        return pRobotMessage->header().seqno();
    }
    else
    {
        printf("No header\n");
        return 0;
    }
}

void Robot_EGM::setOffset(struct OFFSET (offset))
{
    offset_x=offset.x;
    offset_y=offset.y;
    offset_z=offset.z;
    q1=offset.q1;
    q2=offset.q2;
    q3=offset.q3;
    q4=offset.q4;
}

void Robot_EGM::getJoints(struct JOINTS (& joints))
{
    joints.j1=egmjoints.joints(0)*180/3.1415;
    joints.j2=egmjoints.joints(1)*180/3.1415;
    joints.j3=egmjoints.joints(2)*180/3.1415;
    joints.j4=egmjoints.joints(3)*180/3.1415;
    joints.j5=egmjoints.joints(4)*180/3.1415;
    joints.j6=egmjoints.joints(5)*180/3.1415;
}

void Robot_EGM::getSeqno(int &n)
{
    n=connection_seqno;
}
