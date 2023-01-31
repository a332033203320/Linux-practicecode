#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/sem.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<errno.h>

#define SEM_FILE "./SEM_FILE"

typedef union _semun {
  int val;
  struct semid_ds *buf;
  unsigned char *arr;	
} semun;

int init_sem(int id, int val) {
  semun temp;
  temp.val = val;
  if(semctl(id, 0, SETVAL, temp) == -1) {
    perror("init semaphore error\n");
    return -1;
  }
  return 0;
}

int sem_p(int id) {
  struct sembuf buf;
  buf.sem_num = 0;
  buf.sem_op = -1;
  buf.sem_flg = SEM_UNDO;
  if(semop(id, &buf, 1) == -1) {
    perror("P operation error\n");
    return -1;
  }
  return 0;
}

int sem_v(int id) {
 struct sembuf buf;
  buf.sem_num = 0;
  buf.sem_op = 1;
  buf.sem_flg = SEM_UNDO;
  if(semop(id, &buf, 1) == -1) {
    perror("V operation error\n");
    return -1;
  }
  return 0;

}

int del_sem(int id) {
  semun temp;
  if(semctl(id, 0, IPC_RMID, temp) == -1) { 
    perror("delete semaphore error\n");
    return -1;
  }
  return 0;
}

int main() {
  int sem_id;
  pid_t pid;
  pid = fork(); 
  key_t key;
  if((key = ftok(SEM_FILE,'z')) < 0) { 
    perror("ftok error\n");
    goto failed;
  } 	
  if((sem_id = semget(key,1,IPC_CREAT|0666)) == -1) { 
    perror("semget error\n");
    goto failed;
  }
  if(init_sem(sem_id,0) == -1) { goto failed; }
  if(pid == 0) {
    //sleep(2);
    //sem_p(sem_id);
    printf("Child Process id: %d\n", getpid());
    sem_v(sem_id); 	 
    //del_sem(sem_id); 
  }
  else if(pid < 0){ 
    printf("fork error\n");
    goto failed;  
  }
  else {
    sem_p(sem_id);
    printf("Parent Process id: %d\n", getpid());
    sem_v(sem_id);
    del_sem(sem_id);
  }
  return 0;
failed:
  exit(1);
}
