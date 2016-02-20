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
struct msgbufDoctor {
  long type;
  long PID;
  int command;
  int ID;
  char name [100];
}messageReceivedDoctor, messageToSendDoctor;

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

int main(int argc, char* argv[]){
  srand(time(NULL));
  int doctorQueueId = msgget(9874, 0777 | IPC_CREAT);
  long doctorPID = getpid();
  while(1){
      int choice = 0;
      messageToSendDoctor.type = 1;
      messageToSendDoctor.command = -1;
      messageToSendDoctor.PID = doctorPID;
      strcpy(messageToSendDoctor.name, createRandomName());
      int ID = 0;
      msgsnd(doctorQueueId, &messageToSendDoctor, sizeof(messageToSendDoctor) - sizeof(long), 0);
      
      msgrcv(doctorQueueId, &messageReceivedDoctor, sizeof(messageReceivedDoctor) - sizeof(long), doctorPID, 0);
      printf("%s ---- %d ---- %d\n", "Success ", 1, messageReceivedDoctor.ID);
      
      printf("%s\n", "0: Take day off"); 
      printf("%s\n", "2: show list of free terms"); 

      scanf("%d", &choice);
      char tempMessage[100]; 
      switch(choice)
      {
        case 0:
          printf("%s\n", "starting year");
          printf("%s\n", "starting month");
          printf("%s\n", "starting day");
          printf("%s\n", "how much days");
          // 
        break;
       
        case 1:
        break;

        case 2:
        break;
       
        default:
          printf("%s\n", "choose other option"); 
    }
  }
  return 0;
}
