#ifndef GLOBAL_H
#define GLOBAL_H
// Declaring global variables
struct msgbufPatient messageReceivedPatient, messageToSendPatient;
struct msgbufDoctor messageReceivedDoctor, messageToSendDoctor;
struct registration allRegistration[10000];
struct Doctor doctors[100];
struct Patient patients[100];
int numberOfDoctors = 0;
#endif
