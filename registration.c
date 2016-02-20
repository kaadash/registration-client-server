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

struct doctor {
  int doctorPID;
  int numberOfVisits;
  struct registration registrations[100];  
};

struct msgbufPatient {
  long type; // PID of patient
  long PID;
  int command; // type of command
  int isLogged;
  char stringMsgTypeOne[100];
  char stringMsgTypeTwo[100];
  char stringMsgTypeThree[100];
  char longMessage[1000];
}messageReceivedPatient, messageToSendPatient;

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

void intToCharWithIndent(char* text, int number, char* indent) {
  sprintf(text, "%d", number);
  strcat(text, indent);
}

void convertRegistrationToChar(char* listToReturn, struct registration registrationToConvert) {
  char year[16];
  char month[16];
  char day[16];
  char hour[16];
  char doctorPID[32];
  
  intToCharWithIndent(year, registrationToConvert.year, " - ");
  strcat(listToReturn, year);
  
  intToCharWithIndent(month, registrationToConvert.month, " - ");
  strcat(listToReturn, month);
  
  intToCharWithIndent(day, registrationToConvert.day, " hour: ");
  strcat(listToReturn, day);
  
  intToCharWithIndent(hour, registrationToConvert.hour, " doctor ID : ");
  strcat(listToReturn, hour);
  
  intToCharWithIndent(doctorPID, registrationToConvert.doctorPID, " ");
  strcat(listToReturn, doctorPID);

}

char* displayAllFreeTerms(int year, int month, int day) {
  int i;
  char *listToReturn = malloc(1000);
  for (i = 0; i < 100; ++i)
  {
    if(allRegistration[i].isRegistered == 0) {
      if(isSetDate(year, month, day, allRegistration[i]) == 1) {
          convertRegistrationToChar(listToReturn, allRegistration[i]);
          strcat(listToReturn, "\n");
      }
    }
  }
  return listToReturn;
}

void findNewFirstFreeRegistration(int currentYear, int currentMonth, int currentDay){
  int isFound = 0;
  int i;
  for (i = 0; i < 100; ++i)
  {
    if(allRegistration[i].isRegistered == 0) {
      // if(isSetDate(year, month, day, allRegistration[i]) == 1) {
      //     allRegistration[i].day, allRegistration[i].hour, allRegistration[i].doctorPID);
          
      // } 
      if(allRegistration[i].year == currentYear && allRegistration[i].month - currentMonth <= 2 && isFound == 0) {
        isFound = 1;
      }
    }
  }
}

int main(int argc, char* argv[]){
  generateSampleRegistrations();
  int patientQueueId = msgget(9875, 0777 | IPC_CREAT);
  while(1) {
    msgrcv(patientQueueId, &messageReceivedPatient, sizeof(messageReceivedPatient) - sizeof(long), 1, 0);
    if(messageReceivedPatient.isLogged == 0 && messageReceivedPatient.command == -1) {
      printf("pid ::: %ld\n", messageReceivedPatient.PID);
      messageToSendPatient.isLogged = 1;
      messageToSendPatient.type = messageReceivedPatient.PID;
      msgsnd(patientQueueId, &messageToSendPatient, sizeof(messageToSendPatient) - sizeof(long), 0);
    }
    else {
      messageToSendPatient.type = messageReceivedPatient.PID;
      int year;
      int month;
      int day;
      char PESEL[100];
      char name[100];
      char surname[100];
      switch(messageReceivedPatient.command)
      {
        case 0: 
          printf("%s\n", "register meeting");
          // check current date
          // 
          strcpy(PESEL, messageReceivedPatient.stringMsgTypeOne);
          strcpy (name, messageReceivedPatient.stringMsgTypeTwo);
          strcpy(surname, messageReceivedPatient.stringMsgTypeThree); 
        break;
       
        case 1: 
          printf("%s\n", "show list of doctors in some period"); 
        break;

        case 2:
          year = atoi(messageReceivedPatient.stringMsgTypeOne);
          month = atoi(messageReceivedPatient.stringMsgTypeTwo);
          day = atoi(messageReceivedPatient.stringMsgTypeThree);
          strcpy(messageToSendPatient.longMessage, displayAllFreeTerms(year, month, day));
          msgsnd(patientQueueId, &messageToSendPatient, sizeof(messageToSendPatient) - sizeof(long), 0);
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
