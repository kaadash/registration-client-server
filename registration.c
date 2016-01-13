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
printf("%s\n", "Asd");
  // while(1) {
    msgrcv(id, &m1, sizeof(m1) - sizeof(long), 1, 0);
    printf("%s\n", m1.tab);    
  // }
  return 0;
}
