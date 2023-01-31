#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<errno.h>
#include<string.h>
#include"header.h"

#define MFILE "./PASSWORD"

#define error(msg) { \
  perror(msg " error\n"); \
  goto failed; \
}

int init_sem(int semid, int val) {
  semun temp;
  temp.val = val;
  if(semctl(semid,0,SETVAL,temp) == -1) { 
    perror("init semaphore error\n"); 
    return -1; 
  }
  return 0;
}

int del_sem(int semid) {
  semun temp;
  if(semctl(semid,0,IPC_RMID,temp) == -1) { 
    perror("del semaphore error\n"); 
    return -1; 
  }
  return 0;
}

int sem_p(int semid) {
  struct sembuf buf;
  buf.sem_num = 0;
  buf.sem_op = -1;
  buf.sem_flg = SEM_UNDO;
  if(semop(semid,&buf,1) == -1) {
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
  if(semop(semid,&buf,1) == -1) {
    perror("V operation error\n");
    return -1;
  }
  return 0;
}


int main(int argc, char *argv[]) {
  key_t key;
  int msqid, semid, shmid;  
  char *shm = NULL, data[] = "Hello, this is server";
  bool cont = true;
  struct shmid_ds buf1;
  struct msqid_ds buf2; 
  //shared_data  *shared;
  msgform msg;
  if((key = ftok(".",'z')) < 0) { error("ftok"); }  
  if((shmid = shmget(key,1024,IPC_CREAT|0666)) == -1) { error("create shm"); }
  shm = shmat(shmid,0,0);
  if((int)shm == (-1)) { error("attach shm"); }
  if((msqid = msgget(key, IPC_CREAT|0777)) == -1) { error("msgget"); }
  if((semid = semget(key, 1, IPC_CREAT|0666)) == -1) { error("semget"); }
  init_sem(semid,1);
  strcpy(shm, data);
  while(cont) {
    msgrcv(msqid, &msg, 1, 888, 0); 
    if(msg.mtext == 'q') { cont = false; } 
    if(msg.mtext == 'r') {
      sem_p(semid);
      //printf("shm %X", (int)shm);
      printf("%s\n",shm);		
      sem_v(semid);
    }
  }
  shmdt(shm);
  shmctl(shmid, IPC_RMID, &buf1);	
  msgctl(msqid, IPC_RMID, &buf2);
  del_sem(semid);
  return 0;
failed:
  exit(1);
}

