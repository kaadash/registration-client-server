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
  int x;
  char tab[10];
}m1, m2;
int main(int argc, char* argv[]){
  int id = msgget(12345, 0644 | IPC_CREAT);
  m1.type = 1;
  m1.x = atoi(argv[1]);
  strcpy(m1.tab, "Hello");
  msgsnd(id, &m1, sizeof(m1) - sizeof(long), 0);
  return 0;
}
