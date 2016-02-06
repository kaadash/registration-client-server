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
  char msgTypeOne[100];
  char msgTypeTwo[100];
  char msgTypeSecond[100];
  int isLogged;
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

void displayAllFreeTerms() {
  int i;
  for (i = 0; i < 100; ++i)
  {
    if(allRegistration[i].isRegistered == 0) {
      printf("%d-%d-%d godzina:%d - doctor id: %d \n", allRegistration[i].year, allRegistration[i].month,
        allRegistration[i].day, allRegistration[i].hour, allRegistration[i].doctorPID);
      
    }
  }
}

int main(int argc, char* argv[]){
  generateSampleRegistrations();
  int patientQueueId = msgget(9876, 0777 | IPC_CREAT);
  while(1) {
    msgrcv(patientQueueId, &messageReceived, sizeof(messageReceived) - sizeof(long), 0, 0);
    if(messageReceived.isLogged == 0 && messageReceived.command == -1) {
      printf("pid ::: %ld\n", messageReceived.PID);
      messageToSend.isLogged = 1;
      messageToSend.type = messageReceived.PID;
      msgsnd(patientQueueId, &messageToSend, sizeof(messageToSend) - sizeof(long), 0);
    }
    else {
      switch(messageReceived.command)
      {
        case 0: 
          printf("%s\n", "register meeting"); 
        break;
       
        case 1: 
          printf("%s\n", "show list of doctors in some period"); 
        break;

        case 2:
          messageToSend.type = messageReceived.PID;
          msgsnd(patientQueueId, &messageToSend, sizeof(messageToSend) - sizeof(long), 0);
          displayAllFreeTerms();
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
