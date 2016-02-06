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
struct msgbuf {
  long type; // PID of patient
  int command; // type of command
  char msgTypeOne[100];
  char msgTypeTwo[100];
  char msgTypeSecond[100];
  int isLogged;
}message;
// struct msgbuf {
//   long type; <- PID of current patient
//    int command <- type of command - can be registration, showing list of appointment
//   char message[100]; <- there can be more types of messages it depends on my specification
// }message;

int main(int argc, char* argv[]){
  int id = msgget(7777, 0644 | IPC_CREAT);
  int queueTypeId = msgget(7777, 0644 | IPC_CREAT);
  int loggedIn = 0;
  int choice;
  char PESEL[100];
  while(1){
    if(loggedIn != 1) {
      printf("%s\n", "Please log in using your PESEL");
      scanf("%s", PESEL);
      message.type = 1;
      strcpy(message.msgTypeOne, PESEL);
      message.command = 2;
      msgsnd(id, &message, sizeof(message) - sizeof(long), 0);
      msgrcv(id, &message, sizeof(message) - sizeof(long), 2, 0);
      loggedIn = message.isLogged;
    }
    else {
      printf("%s\n", "0: register meeting"); 
      printf("%s\n", "1: show list of doctors in some period"); 
      printf("%s\n", "2: show list of free terms"); 
      printf("%s\n", "3: show list of free terms to specified doctor"); 
      printf("%s\n", "4: show status of meeting"); 
      printf("%s\n", "5: show list of doctors in some period"); 
      printf("%s\n", "6: log out");

      scanf("%d", &choice);
      switch(choice)
      {
        case 0: 
          printf("%s\n", "register meeting"); 
        break;
       
        case 1: 
          printf("%s\n", "show list of doctors in some period"); 
        break;

        case 2: 
          printf("%s\n", "show list of free terms"); 
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
          loggedIn = 0; 
          printf("%s\n", "log out correctly");
        break;
       
        default:
          printf("%s\n", "choose other option"); 
      }
    }
  }
  return 0;
}
