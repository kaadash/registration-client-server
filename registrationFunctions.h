#ifndef REGISTRATION_FUNCTIONS_H
#define REGISTRATION_FUNCTIONS_H
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
#include "structures.h"
#include "global.h"
#include "helpers.h"

void generateSampleRegistrations(int doctorID, int currentYear, int currentMonth, int currentDay, int currentHour) {
  int i = 0;
  int doctorCounter = 0;
  currentHour = 0;
  int endPoint = doctorID * 500;
  int startPoint = doctorID;
  if (startPoint != 0) {
    startPoint = (doctorID * 500) - 500;
  }
  for (i = startPoint; i < endPoint; ++i)
  {
    allRegistration[i].isRegistered = 0;
    allRegistration[i].doctorID = doctorID;
    currentHour += 6;
    if( (i % 4) == 0 ) {
      currentDay++;
      currentHour = 0;
      if (currentDay >= 30)
      {
        currentDay = 1;
        currentMonth++;
        if (currentMonth >= 13) {
          currentMonth = 1;
          currentYear++;
        }
      }
    }

    allRegistration[i].hour = currentHour;
    allRegistration[i].day = currentDay;
    allRegistration[i].month = currentMonth;
    allRegistration[i].year = currentYear;
    doctors[doctorID].registrations[doctorCounter] = allRegistration[i];
    doctorCounter++;
  }
}

char* displayAllFreeTerms(int year, int month, int day) {
  int i;
  char *listToReturn = malloc(100000);
  for (i = 0; i < numberOfDoctors * 500; ++i)
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

char* displayAllFreeTermsBySpecDoctor(int doctorID, int currentYear, int currentMonth, int currentDay) {
  int i;
  printf("----------------------  %d\n", doctorID);
  char *listToReturn = malloc(100000);
  for (i = 0; i < 500; ++i)
  {
    if(doctors[doctorID].registrations[i].isRegistered == 0 && doctors[doctorID].registrations[i].month == currentMonth && doctors[doctorID].registrations[i].year == currentYear && doctors[doctorID].registrations[i].day >= currentDay) {
          convertRegistrationToChar(listToReturn, doctors[doctorID].registrations[i]);
          strcat(listToReturn, "\n");
    }
  }
  return listToReturn;
}

char* displayAllDoctors(int year, int month, int day) {
  int i;
  char text[100];
  char *listToReturn = malloc(100000);
  int doctorsIDs[100];
  int doctorsIDsCounter = 0;
  for (i = 0; i < numberOfDoctors * 500; ++i)
  {
    if(isSetDate(year, month, day, allRegistration[i]) == 1) {
      int j = 0;
      int isFound = 0;
      for (j = 0; j < numberOfDoctors; ++j)
      {
          if(allRegistration[i].doctorID == doctorsIDs[j]) 
            isFound = 1;
      }
      if(isFound == 0) {
        doctorsIDs[doctorsIDsCounter] = allRegistration[i].doctorID;
        strcat(listToReturn, "doctor ID: ");
        intToCharWithIndent(text, allRegistration[i].doctorID, "name: ");
        strcat(listToReturn, text);
        strcat(listToReturn, doctors[allRegistration[i].doctorID].name);
        strcat(listToReturn, "\n");
        doctorsIDsCounter++;
      }
    }
  }
  return listToReturn;
}


char* displayStatusOfRegistration(int year, int month, int day, int hour, int doctorID) {
  int i = 0;
  int isFound = 0;
  char *listToReturn = malloc(1000);
  
  while (i < numberOfDoctors * 500 && isFound == 0)
  {
      if(isSetDateWithTime(year, month, day, hour, allRegistration[i]) == 1 && allRegistration[i].doctorID == doctorID) {
          convertRegistrationToChar(listToReturn, allRegistration[i]);
          strcat(listToReturn, "status: \n");
          if(allRegistration[i].isRegistered == 0) {
            strcat(listToReturn, "it is not registered yet \n");
          }
          else {
            strcat(listToReturn, "it is registered by: \n");
            strcat(listToReturn, allRegistration[i].patientName);
            strcat(listToReturn, allRegistration[i].patientSurname);
            strcat(listToReturn, allRegistration[i].patientPESEL);
          }
          isFound = 1;
      }
      i++;
  }
  if(isFound == 0) {
    strcat(listToReturn, "not found registration with this doctor ID and this date. Please try again \n");
  }
  printf("%s\n", listToReturn);

  return listToReturn;
}

char* cancelRegistration(int year, int month, int day, int hour, int doctorID, long patientPID) {
  int i = 0;
  int isFound = 0;
  char *listToReturn = malloc(1000);
  printf("doctorID %d\n", doctorID);
  while (i < numberOfDoctors * 500 && isFound == 0)
  {
    printf("doctorID %d\n", allRegistration[i].doctorID);
      if(isSetDateWithTime(year, month, day, hour, allRegistration[i]) == 1 && allRegistration[i].patientPID == patientPID && allRegistration[i].isRegistered == 1 && allRegistration[i].doctorID == doctorID) {
          convertRegistrationToChar(listToReturn, allRegistration[i]);
          allRegistration[i].isRegistered = 0;
          isFound = 1;
      }
      i++;
  }
  if(isFound == 0) {
    strcat(listToReturn, "not found registration with this doctor ID and this date. Please try again \n");
  }
  else {
    strcat(listToReturn, "Successfully cancel registration \n"); 
  }
  printf("%s\n", listToReturn);

  return listToReturn;
}

char* findNewFirstFreeRegistration(int currentYear, int currentMonth, int currentDay, long patientPID, char* PESEL, char* surname, char* name){
  int isFound = 0;
  int i = 0;
  char *communicationAboutRegistration = malloc(1000);
  while (i < numberOfDoctors * 500 && isFound == 0)
  {
    if(allRegistration[i].isRegistered == 0) {
      if(allRegistration[i].year == currentYear && allRegistration[i].month - currentMonth <= 2 && 
        allRegistration[i].month >= currentMonth + 1 && allRegistration[i].day >= currentDay) {

          allRegistration[i].isRegistered = 1;
          allRegistration[i].patientPID = patientPID;
          strcpy(allRegistration[i].patientPESEL, PESEL);
          strcpy(allRegistration[i].patientSurname, surname);
          strcpy(allRegistration[i].patientName, name);
          int j;
          int doctorID = allRegistration[i].doctorID;
          for (j = 0; j < 500; ++j)
          {
            int year = allRegistration[i].year;
            int month = allRegistration[i].month;
            int day = allRegistration[i].day;
            int hour = allRegistration[i].hour;
            if(isSetDateWithTime(year, month, day, hour, doctors[doctorID].registrations[j])) {
              doctors[doctorID].registrations[j].isRegistered = 1;
            }
          }

          convertRegistrationToChar(communicationAboutRegistration, allRegistration[i]);
          isFound = 1;
      }
    }
    i++;
  }
  return communicationAboutRegistration;
}
char* changeDateOfRegistration(int year, int month, int day, int hour, int doctorID, long patientPID, int currentYear, int currentMonth, int currentDay) {
  int i = 0;
  int isFound = 0;
  char *listToReturn = malloc(1000);
  while (i < numberOfDoctors * 500 && isFound == 0)
  {
    if(isSetDateWithTime(year, month, day, hour, allRegistration[i]) == 1 && allRegistration[i].patientPID == patientPID && allRegistration[i].isRegistered == 1 && allRegistration[i].doctorID == doctorID) {
        allRegistration[i].isRegistered = 0;
        isFound = 1;
        strcat(listToReturn, findNewFirstFreeRegistration(currentYear, currentMonth, currentDay, patientPID, allRegistration[i].patientPESEL, allRegistration[i].patientSurname, allRegistration[i].patientName));
          printf("----------------%s\n", allRegistration[i].patientPESEL);
        strcpy(allRegistration[i].patientPESEL, ""); 
        strcpy(allRegistration[i].patientName, ""); 
        strcpy(allRegistration[i].patientSurname, ""); 
    }
    i++;
  }
  if(isFound == 0) {
    strcat(listToReturn, "not found registration with this doctor ID and this date. Please try again \n");
  }

  return listToReturn;
}

char* takeDayOff(int year, int month, int day, int numberOfDays, int doctorID) {
  int i = 0;
  int dayCounter = 0;
  char *listToReturn = malloc(1000);
  while (i < numberOfDoctors * 500 && dayCounter < numberOfDays)
  {
    if(isSetDate(year, month, day + dayCounter, allRegistration[i]) == 1 && allRegistration[i].doctorID == doctorID) {
        printf("days off: %d - %d\n", month, day + dayCounter);
        allRegistration[i].isRegistered = 1;
        strcpy(allRegistration[i].patientPESEL, ""); 
        strcpy(allRegistration[i].patientName, ""); 
        strcpy(allRegistration[i].patientSurname, ""); 
        dayCounter++;
    }
    i++;
  }
  strcpy(listToReturn, "Success");
  return listToReturn;
}

#endif