#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>  
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "structures.h"
#include "helpers.h"

struct msgbufDoctor messageReceivedDoctor, messageToSendDoctor;


void insertDateAndNumberOfDays() {
  char tempMessage[100];
  printf("%s\n", "Write year: ");
  scanf("%s", tempMessage);
  strcpy(messageToSendDoctor.stringMsgTypeOne, tempMessage);
  printf("%s\n", "Write month: ");
  scanf("%s", tempMessage);
  strcpy(messageToSendDoctor.stringMsgTypeTwo, tempMessage);
  printf("%s\n", "Write day: ");
  scanf("%s", tempMessage);
  strcpy(messageToSendDoctor.stringMsgTypeThree, tempMessage);
  printf("%s\n", "Write number of days: ");
  scanf("%d", &messageToSendDoctor.intMessage);
}

int main(int argc, char* argv[]){
  srand(time(NULL));
  int doctorQueueId = msgget(9874, 0777 | IPC_CREAT);
  long doctorPID = getpid();
  messageToSendDoctor.type = 1;
  messageToSendDoctor.command = -1;
  messageToSendDoctor.PID = doctorPID;
  strcpy(messageToSendDoctor.name, createRandomName());
  int ID = 0;
  msgsnd(doctorQueueId, &messageToSendDoctor, sizeof(messageToSendDoctor) - sizeof(long), 0);
  msgrcv(doctorQueueId, &messageReceivedDoctor, sizeof(messageReceivedDoctor) - sizeof(long), doctorPID, 0);
  ID = messageReceivedDoctor.ID;
  printf("doctor ID: %d\n", ID);
  while(1){
      int choice = 0;
      printf("%s\n", "0: Take day off"); 

      scanf("%d", &choice);
      switch(choice)
      {
        case 0:
          insertDateAndNumberOfDays();
          messageToSendDoctor.command = 0;
          messageToSendDoctor.ID = ID;
          msgsnd(doctorQueueId, &messageToSendDoctor, sizeof(messageToSendDoctor) - sizeof(long), 0);
          msgrcv(doctorQueueId, &messageReceivedDoctor, sizeof(messageReceivedDoctor) - sizeof(long), doctorPID, 0);
          printf("%s\n", messageReceivedDoctor.longMessage);
        break;

        default:
          printf("%s\n", "choose other option"); 
          break;
    }
  }
  return 0;
}
