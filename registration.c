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
  long type;
  char PESEL[100];
  char PID[100];
  int isLogged;
  char message[100];
}m1;
int main(int argc, char* argv[]){
int id = msgget(7777, 0644 | IPC_CREAT);
  msgrcv(id, &m1, sizeof(m1) - sizeof(long), 1, 0);
  printf("%s\n", m1.PESEL);    
  printf("%s\n", m1.PID);
  m1.isLogged = 1;
  m1.type = 2;  
  msgsnd(id, &m1, sizeof(m1) - sizeof(long), 0);
  return 0;
}
