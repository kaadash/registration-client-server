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
  char name [100];
}messageReceivedDoctor, messageToSendDoctor;;
int main(int argc, char* argv[]){
  int id = msgget(12345, 0644 | IPC_CREAT);
  while(1){
      int choice = 0;
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
