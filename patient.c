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
  char tab[100];
}m1;
int main(int argc, char* argv[]){
  int id = msgget(7777, 0644 | IPC_CREAT);
  m1.type = 1;
  // msgsnd(id, &m1, sizeof(m1) - sizeof(long), 0);
  int choice;
  char PESEL[100];
  while(1){
    printf("%s\n", "Please log in using your PESEL");
    scanf("%s", PESEL);
    strcpy(m1.tab, PESEL);
    printf("%s\n", m1.tab);
    msgsnd(id, &m1, sizeof(m1) - sizeof(long), 0);
  }
  return 0;
}
