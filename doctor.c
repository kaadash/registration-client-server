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
int main(int argc, char* argv[]){
  int doctorQueueId = msgget(9874, 0777 | IPC_CREAT);
  long doctorPID = getpid();
  while(1){
      int choice = 0;
      messageToSendDoctor.type = 1;
      messageToSendDoctor.command = -1;
      messageToSendDoctor.PID = doctorPID;
      strcpy(messageToSendDoctor.name, "Artrur");
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
