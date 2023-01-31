#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/msg.h>
#include<error.h>
#include<string.h>
#include"header.h"

#define MFILE "./PASSWORD"

#define error(msg) { \
  perror(msg " error\n"); \
  goto failed; \
}

int sem_p(int semid) {
  struct sembuf buf;
  buf.sem_num = 0;
  buf.sem_op = -1;
  buf.sem_flg = SEM_UNDO;  
  if(semop(semid, &buf, 1) == -1) {
    perror("P operation error\n");
    return -1;
  }
  return 0;
}

int sem_v(int semid) {
  struct sembuf buf;
  buf.sem_num = 0;
  buf.sem_op = 1;
  buf.sem_flg = SEM_UNDO;  
  if(semop(semid, &buf, 1) == -1) {
    perror("V operation error\n");
    return -1;
  }
  return 0;
}

void send(const char ch, int msqid, msgform *msg) {
  msg->mtype = 888;
  msg->mtext = ch;
  msgsnd(msqid, msg, sizeof(msg->mtext), 0);	
}

void flush() {
  char ch;
  while((ch = getchar()) != '\n' && ch != EOF);
}

int main(int argc, char *arv[]) {
  key_t key;
  int shmid, semid, msqid;
  char *shm;
  bool cont = true;
  msgform msg;
  if((key = ftok(".",'z')) < 0) { error("ftok"); }
  if((shmid = shmget(key,1024,0)) == -1) { error("shmget"); }  
  shm = (char*)shmat(shmid,0,0);
  if((int)shm == -1) { error("attached shm"); }
  if((msqid = msgget(key, 0)) == -1) { error("msgget"); }
  if((semid = semget(key, 0, 0)) == -1) { error("semget"); }
  printf("%s\n", shm);
  printf("(r)Send data, (q)Quit\n");
  while(cont) {
    char ch = getchar();
    switch(ch) {
      case 'r':
        printf("Data to send\n");
        sem_p(semid);
	scanf("%s", shm);
        sem_v(semid);
        send('r', msqid, &msg);
        break;
      case 'q':	
        send('q', msqid, &msg);
        cont = false;
        break;
      default:
       printf("invalid command\n");		
    }
    flush();
  }
  return 0;
failed:
  exit(1);
}
