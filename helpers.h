#ifndef HELPERS_H
#define HELPERS_H
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>  
#include <string.h>

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


void updateDateAndTime(time_t baseTime, int* currentHour, int* currentDay, int* currentMonth,  int* currentYear) {
  time_t currentTime = time(NULL);
  int diffTime = (currentTime - baseTime) * 2;
  printf("%d\n", diffTime);
  *currentHour = diffTime % 24;
  *currentDay = (*currentDay + (diffTime / 24)) % 30;
  *currentMonth = (*currentMonth + ((*currentDay + (diffTime / 24)) / 30)) % 12;
  *currentYear = *currentYear + (*currentMonth + ((*currentDay + (diffTime / 24)) / 30)) / 12;
}

char* createRandomName() {
  int randomNumber = rand() % 4;
  int i;
  char *listToReturn = malloc(100);
  char firstName [] = "Anna";
  char secondName [] = "Jan";
  char thirdName [] = "Gustaw";
  char forthName [] = "Karolina";
  for (i = 0; i < 2; ++i)
  {
    randomNumber = rand() % 4;
    switch(randomNumber) {
      case 0:
        strcat(listToReturn, firstName);
      break;

      case 1:
        strcat(listToReturn, secondName);
      break;

      case 2:
        strcat(listToReturn, thirdName);
      break;

      case 3:
        strcat(listToReturn, forthName);
      break;

      default:
      break;
    }
    strcat(listToReturn, " ");
  }
  return listToReturn;
}

#endif