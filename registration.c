#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
struct registration {
  int isRegistered;
  int patientPID;
  char patientPESEL[100];
  char patientName[100];
  char patientSurname[100];
  int doctorPID;
  int day;
  int month;
  int year;
  int hour;
};

struct msgbuf {
  long type; // PID of patient
  long PID;
  int command; // type of command
  int isLogged;
  char stringMsgTypeOne[100];
  char stringMsgTypeTwo[100];
  char stringMsgTypeThree[100];
  char longMessage[1000];
}messageReceived, messageToSend;

struct registration allRegistration[100];

void generateSampleRegistrations() {
  int i = 0;
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  int hour = 12;
  int day = tm.tm_mday;
  int month = tm.tm_mon + 1;
  int year = tm.tm_year + 1900;
  // int currentHour = tm.tm_hour;
  for (i = 0; i < 100; ++i)
  {
    allRegistration[i].isRegistered = 0;
    allRegistration[i].doctorPID = i % 10;
    hour = ((i % 2) + 1 )* 12;
    if( (i % 2) + 1 == 2 ) {
      day+=3;
      if (day >= 30)
      {
        day = 1;
        month++;
        if (month >= 13) {
          month = 1;
          year++;
        }
      }
    }

    allRegistration[i].hour = hour;
    allRegistration[i].day = day;
    allRegistration[i].month = month;
    allRegistration[i].year = year;

    // printf("now: %d-%d-%d %d:hour\n", allRegistration[i].year, allRegistration[i].month,
    //  allRegistration[i].day, allRegistration[i].hour);
  }
}

int isSetDate(int year, int month, int day, struct registration regist) {
  if(regist.year == year && regist.month == month && regist.day == day) {
    return 1;
  }
  else {
    return 0;
  }
}

char* displayAllFreeTerms(int year, int month, int day) {
  int i;
  char listToReturn[1000];
  for (i = 0; i < 100; ++i)
  {
    if(allRegistration[i].isRegistered == 0) {
      printf("---------------->>>>>>>>>>>>>%d-%d-%d godzina:%d - doctor id: %d \n", allRegistration[i].year, allRegistration[i].month,
          allRegistration[i].day, allRegistration[i].hour, allRegistration[i].doctorPID);
      if(isSetDate(year, month, day, allRegistration[i]) == 1) {
        printf("%d-%d-%d godzina:%d - doctor id: %d \n", allRegistration[i].year, allRegistration[i].month,
          allRegistration[i].day, allRegistration[i].hour, allRegistration[i].doctorPID);
        // strcpy(listToReturn, allRegistration[i].doctorPID + '\0');
        // listToReturn += allRegistration[i].doctorPID;
      } 
    }
  }
  return listToReturn;
}

int main(int argc, char* argv[]){
  generateSampleRegistrations();
  int patientQueueId = msgget(9875, 0777 | IPC_CREAT);
  while(1) {
    msgrcv(patientQueueId, &messageReceived, sizeof(messageReceived) - sizeof(long), 1, 0);
    if(messageReceived.isLogged == 0 && messageReceived.command == -1) {
      printf("pid ::: %ld\n", messageReceived.PID);
      messageToSend.isLogged = 1;
      messageToSend.type = messageReceived.PID;
      msgsnd(patientQueueId, &messageToSend, sizeof(messageToSend) - sizeof(long), 0);
    }
    else {
      messageToSend.type = messageReceived.PID;
      int year;
      int month;
      int day;
      switch(messageReceived.command)
      {
        case 0: 
          printf("%s\n", "register meeting"); 
        break;
       
        case 1: 
          printf("%s\n", "show list of doctors in some period"); 
        break;

        case 2:
          year = atoi(messageReceived.stringMsgTypeOne);
          month = atoi(messageReceived.stringMsgTypeTwo);
          day = atoi(messageReceived.stringMsgTypeThree);
          msgsnd(patientQueueId, &messageToSend, sizeof(messageToSend) - sizeof(long), 0);
          displayAllFreeTerms(year, month, day);
        break;

        case 3: 
          printf("%s\n", "show list of free terms to specified doctor"); 
        break;
        
        case 4: 
          printf("%s\n", "show status of meeting"); 
        break;
        
        case 5: 
          printf("%s\n", "show list of doctors in some period"); 
        break;

        case 6: 
          printf("%s\n", "log out correctly");
        break;
       
        default:
          printf("%s\n", "choose other option"); 
      }
    }
  }
  return 0;
}
