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
#include "registrationFunctions.h"

int main(int argc, char* argv[]){
  int patientQueueId = msgget(9875, 0777 | IPC_CREAT);
  int doctorQueueId = msgget(9874, 0777 | IPC_CREAT);
  int numberOfPatients = 0;

  time_t baseTime = time(NULL);
  struct tm tm = *localtime(&baseTime);
  int currentHour = tm.tm_hour;
  int currentDay = tm.tm_mday;
  int currentMonth = tm.tm_mon + 1;
  int currentYear = tm.tm_year + 1900;

  int year;
  int month;
  int day;
  int hour;
  int numberOfDays;
  char PESEL[100];
  char name[100];
  char surname[100];

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
      generateSampleRegistrations(numberOfDoctors, currentYear, currentMonth, currentDay, currentHour);
    }

    if(messageReceivedDoctor.command == 0) {
        messageToSendDoctor.type = messageReceivedDoctor.PID;
        year = atoi(messageReceivedDoctor.stringMsgTypeOne);
        month = atoi(messageReceivedDoctor.stringMsgTypeTwo);
        day = atoi(messageReceivedDoctor.stringMsgTypeThree);
        numberOfDays = messageReceivedDoctor.intMessage;
        strcpy(messageToSendDoctor.longMessage, takeDayOff(year, month, day, numberOfDays, messageReceivedDoctor.ID));
        msgsnd(doctorQueueId, &messageToSendDoctor, sizeof(messageToSendDoctor) - sizeof(long), 0);
    }
    
    msgrcv(patientQueueId, &messageReceivedPatient, sizeof(messageReceivedPatient) - sizeof(long), 1, 0);
    messageToSendPatient.type = messageReceivedPatient.PID;
    
    if(messageReceivedPatient.isLogged == 0 && messageReceivedPatient.command == -1) {
      printf("pid ::: %ld\n", messageReceivedPatient.PID);
      char password[10];
      sprintf(password, "%ld", messageReceivedPatient.PID);
      if(strcmp(messageReceivedPatient.stringMsgTypeOne, password)){
        messageToSendPatient.isLogged = 0;
      }
      else {
        messageToSendPatient.isLogged = 1;
        patients[numberOfPatients].ID = messageReceivedPatient.PID;
        patients[numberOfPatients].isLogged = 1;
      }
      msgsnd(patientQueueId, &messageToSendPatient, sizeof(messageToSendPatient) - sizeof(long), 0);
    }
    else {
      updateDateAndTime(baseTime, &currentHour, &currentDay, &currentMonth,  &currentYear);
      // For displaying current date and test
      printf("hour:  %d\n", currentHour);
      printf("day:  %d\n", currentDay);
      printf("month:  %d\n", currentMonth);
      printf("month:  %d\n", currentYear);

      messageToSendPatient.type = messageReceivedPatient.PID;
      switch(messageReceivedPatient.command)
      {
        case 0: 
          printf("%s\n", "register meeting");
          strcpy(PESEL, messageReceivedPatient.stringMsgTypeOne);
          strcpy (name, messageReceivedPatient.stringMsgTypeTwo);
          strcpy(surname, messageReceivedPatient.stringMsgTypeThree);
          strcpy(messageToSendPatient.longMessage, findNewFirstFreeRegistration(currentYear, currentMonth, currentDay, messageReceivedPatient.PID, PESEL, surname, name));
          printf("%s\n", messageToSendPatient.longMessage);
          msgsnd(patientQueueId, &messageToSendPatient, sizeof(messageToSendPatient) - sizeof(long), 0); 
        break;
       
        case 1: 
          printf("%s\n", "show list of doctors in some period");
          year = atoi(messageReceivedPatient.stringMsgTypeOne);
          month = atoi(messageReceivedPatient.stringMsgTypeTwo);
          day = atoi(messageReceivedPatient.stringMsgTypeThree);
          strcpy(messageToSendPatient.longMessage, displayAllDoctors(year, month, day));
          msgsnd(patientQueueId, &messageToSendPatient, sizeof(messageToSendPatient) - sizeof(long), 0); 
        break;

        case 2:
          year = atoi(messageReceivedPatient.stringMsgTypeOne);
          month = atoi(messageReceivedPatient.stringMsgTypeTwo);
          day = atoi(messageReceivedPatient.stringMsgTypeThree);
          strcpy(messageToSendPatient.longMessage, displayAllFreeTerms(year, month, day));
          msgsnd(patientQueueId, &messageToSendPatient, sizeof(messageToSendPatient) - sizeof(long), 0);
        break;

        case 3: 
          strcpy(messageToSendPatient.longMessage, displayAllFreeTermsBySpecDoctor(messageReceivedPatient.intMessage, currentYear, currentMonth, currentDay));
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
          printf("%s\n", messageToSendPatient.longMessage);
          msgsnd(patientQueueId, &messageToSendPatient, sizeof(messageToSendPatient) - sizeof(long), 0);
        break;
        
        case 5: 
          printf("%s\n", "cancel registration");
          year = atoi(messageReceivedPatient.stringMsgTypeOne);
          month = atoi(messageReceivedPatient.stringMsgTypeTwo);
          day = atoi(messageReceivedPatient.stringMsgTypeThree);
          hour = atoi(messageReceivedPatient.longMessage);
          strcpy(messageToSendPatient.longMessage, cancelRegistration(year, month, day, hour, messageReceivedPatient.intMessage, messageReceivedPatient.PID));
          msgsnd(patientQueueId, &messageToSendPatient, sizeof(messageToSendPatient) - sizeof(long), 0);
        break;

        case 6: 
          printf("%s\n", "change date of registration");
          year = atoi(messageReceivedPatient.stringMsgTypeOne);
          month = atoi(messageReceivedPatient.stringMsgTypeTwo);
          day = atoi(messageReceivedPatient.stringMsgTypeThree);
          hour = atoi(messageReceivedPatient.longMessage);
          updateDateAndTime(baseTime, &currentHour, &currentDay, &currentMonth,  &currentYear);
          strcpy(messageToSendPatient.longMessage, changeDateOfRegistration(year, month, day, hour, messageReceivedPatient.intMessage, messageReceivedPatient.PID, currentYear, currentMonth, currentDay));
          msgsnd(patientQueueId, &messageToSendPatient, sizeof(messageToSendPatient) - sizeof(long), 0);
        break;

        case 7: 
          printf("%s\n", "log out correctly");
        break;
       
        default:
          printf("%s\n", "choose other option"); 
          break;
      }
    }
  }
  return 0;
}
