#include<stdio.h>
#include<stdlib.h>
#include<sys/msg.h>
#include<unistd.h>

#define MSG_FILE "/etc/passwd-"

typedef struct my_msg {
  long mtype;
  char mtext[256];
} my_msg;

int main(int argc, char* argv[]) {
  int msqid;
  key_t key;
  my_msg *msg = (my_msg*)malloc(sizeof(my_msg));
  if((key = ftok(MSG_FILE,'z')) < 0) { perror("ftok error\n"); exit(1); }
  printf("Message Queue -- Clinet key is:%d\n", key);

  if((msqid = msgget(key,IPC_CREAT|0777)) == -1) {
    perror("msgget error\n");
    exit(1);
  }
 
  printf("My msqid is: %d\n", msqid);
  printf("My pid is: %d\n", getpid());
  //send message
  msg->mtype = 888;
  sprintf(msg->mtext, "Hello, this's client pid: %d\n", getpid());
  msgsnd(msqid, msg, sizeof(msg->mtext),0);  
  //receive message
  msgrcv(msqid, msg, 256, 999, 0);
  printf("Client receive: (%s:%ld) \n", msg->mtext, msg->mtype);
  return 0;
}
