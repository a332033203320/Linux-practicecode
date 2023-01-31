#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
#include<sys/msg.h>

#define MSG_FILE "/etc/passwd-"


typedef struct my_msg {
  long mtype;
  char mtext[256];
} my_msg;

int main(int argc, char* argv[]) {
  int msqid;
  key_t key;
  my_msg *msg = (my_msg*)malloc(sizeof(my_msg));
  if((key = ftok(MSG_FILE,'z')) < 0) { perror("ftok error"); exit(1); } 
  printf("Message Queue-- Server key is: %d\n", key);
  if((msqid = msgget(key, IPC_CREAT|0777)) == -1) { 
    perror("msgget error");
    exit(1);
  }
  printf("My msqid: %d\n", msqid);
  printf("My pid: %d\n", getpid());
  bool cont = true;
  while(cont) {
    msgrcv(msqid,msg,256,888,0);
    printf("Server->receive message: (%s:%ld)\n", msg->mtext, msg->mtype);
    msg->mtype = 999;
    sprintf(msg->mtext, "Hello, this's server pid: %d", getpid());
    msgsnd(msqid, msg, sizeof(msg->mtext), 0);
  }
  return 0;
}
