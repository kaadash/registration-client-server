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
  struct registration registrations[200];  
};

struct msgbufPatient {
  long type; // PID of patient
  long PID;
  int command; // type of command
  int isLogged;
  int intMessage;
  char stringMsgTypeOne[100];
  char stringMsgTypeTwo[100];
  char stringMsgTypeThree[100];
  char longMessage[1000];
}messageReceivedPatient, messageToSendPatient;

struct msgbufDoctor {
  long type;
  long PID;
  int command;
  int ID;
  char name [100];
}messageReceivedDoctor, messageToSendDoctor;


struct registration allRegistration[10000];
struct Doctor doctors[100];
int numberOfDoctors = 0;


void generateSampleRegistrations(int doctorID) {
  int i = 0;
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  int hour = 12;
  int day = tm.tm_mday;
  int month = tm.tm_mon + 1;
  int year = tm.tm_year + 1900;
  int endPoint = doctorID * 100;
  int startPoint = doctorID;
  if (startPoint != 0) {
    startPoint = (doctorID * 100) - 100;
  }
  for (i = startPoint; i < endPoint; ++i)
  {
    allRegistration[i].isRegistered = 0;
    allRegistration[i].doctorID = doctorID;
    hour += 6;
    if( (i % 4) == 0 ) {
      day++;
      hour = 0;
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
    doctors[doctorID].registrations[i] = allRegistration[i];
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

int isSetDateWithTime(int year, int month, int day, int hour, struct registration regist) {
  if(regist.year == year && regist.month == month && regist.day == day && regist.hour == hour) {
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
  char doctorID[32];
  
  intToCharWithIndent(year, registrationToConvert.year, " - ");
  strcat(listToReturn, year);
  
  intToCharWithIndent(month, registrationToConvert.month, " - ");
  strcat(listToReturn, month);
  
  intToCharWithIndent(day, registrationToConvert.day, " hour: ");
  strcat(listToReturn, day);
  
  intToCharWithIndent(hour, registrationToConvert.hour, " doctor ID : ");
  strcat(listToReturn, hour);
  
  intToCharWithIndent(doctorID, registrationToConvert.doctorID, " ");
  strcat(listToReturn, doctorID);

}

char* displayAllFreeTerms(int year, int month, int day) {
  int i;
  char *listToReturn = malloc(100000);
  for (i = 0; i < numberOfDoctors * 100; ++i)
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

char* displayAllFreeTermsBySpecDoctor(int doctorID) {
  int i;
  char *listToReturn = malloc(100000);
  for (i = 0; i < 100; ++i)
  {
    if(doctors[doctorID].registrations[i].isRegistered == 0) {
          convertRegistrationToChar(listToReturn, doctors[doctorID].registrations[i]);
          strcat(listToReturn, "\n");
    }
  }
  return listToReturn;
}

char* displayStatusOfRegistration(int year, int month, int day, int hour, int doctorID) {
  int i = 0;
  int isFound = 0;
  char *listToReturn = malloc(1000);
  
  while (i < numberOfDoctors * 100 && isFound == 0)
  {
      if(isSetDateWithTime(year, month, day, hour, allRegistration[i]) == 1 && allRegistration[i].doctorID == doctorID) {
          convertRegistrationToChar(listToReturn, allRegistration[i]);
          strcat(listToReturn, "status: \n");
          if(allRegistration[i].isRegistered == 0) {
            strcat(listToReturn, "it is not registered yet \n");
          }
          else {
            strcat(listToReturn, "it is registered try other \n");
          }
          isFound = 1;
      }
      i++;
  }
  if(isFound == 0) {
    strcat(listToReturn, "not found registration with this doctor ID and this date. Please try again \n");
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
      //     allRegistration[i].day, allRegistration[i].hour, allRegistration[i].doctorID);
          
      // } 
      if(allRegistration[i].year == currentYear && allRegistration[i].month - currentMonth <= 2 && isFound == 0) {
        isFound = 1;
      }
    }
  }
}
int main(int argc, char* argv[]){
  int patientQueueId = msgget(9875, 0777 | IPC_CREAT);
  int doctorQueueId = msgget(9874, 0777 | IPC_CREAT);
  while(1) {
    
    while (msgrcv(doctorQueueId, &messageReceivedDoctor, sizeof(messageReceivedDoctor) - sizeof(long), 1, IPC_NOWAIT) != -1 && messageReceivedDoctor.command == -1)
    {
      printf("%s\n", "new doctor appeared");
      messageToSendDoctor.type = messageReceivedDoctor.PID;
      messageToSendDoctor.ID = numberOfDoctors;
      msgsnd(doctorQueueId, &messageToSendDoctor, sizeof(messageToSendDoctor) - sizeof(long), 0);
      
      doctors[numberOfDoctors].ID = numberOfDoctors;
      strcpy(doctors[numberOfDoctors].name, messageReceivedDoctor.name);
      printf("number of doctors: %d\n", numberOfDoctors);
      numberOfDoctors++;
      generateSampleRegistrations(numberOfDoctors);
    }
    
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
      int hour;
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
          strcpy(messageToSendPatient.longMessage, displayAllFreeTermsBySpecDoctor(messageReceivedPatient.intMessage));
          printf("%s\n", messageToSendPatient.longMessage);
          msgsnd(patientQueueId, &messageToSendPatient, sizeof(messageToSendPatient) - sizeof(long), 0);
          printf("%s\n", "show list of free terms to specified doctor"); 
        break;
        
        case 4: 
          printf("%s\n", "show status of meeting"); 
          year = atoi(messageReceivedPatient.stringMsgTypeOne);
          month = atoi(messageReceivedPatient.stringMsgTypeTwo);
          day = atoi(messageReceivedPatient.stringMsgTypeThree);
          hour = atoi(messageReceivedPatient.longMessage);
          strcpy(messageToSendPatient.longMessage, displayStatusOfRegistration(year, month, day, hour, messageReceivedPatient.intMessage));
          msgsnd(patientQueueId, &messageToSendPatient, sizeof(messageToSendPatient) - sizeof(long), 0);
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
