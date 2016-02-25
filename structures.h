#ifndef STRUCTURES_H
#define STRUCTURES_H

struct registration {
  int isRegistered;
  int patientPID;
  char patientPESEL[100];
  char patientName[100];
  char patientSurname[100];
  int doctorID;
  int day;
  int month;
  int year;
  int hour;
};

struct Doctor {
  int ID;
  char name[100];
  int numberOfVisits;
  struct registration registrations[500];
};

struct Patient {
  int ID;
  int isLogged;
};

struct msgbuf {
  long type; // PID of patient
  long PID; 
  int command; // type of command
  int isLogged;
  int intMessage;
  char stringMsgTypeOne[100];
  char stringMsgTypeTwo[100];
  char stringMsgTypeThree[100];
  char longMessage[1000];
};

#endif