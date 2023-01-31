#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<stdbool.h>
#include<sys/types.h>
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>

void handler(int signum) {
  printf("receive singal number: %d\n", signum);
  if(signum == SIGINT) {
    printf("ctrl+c executes for main process\n");
    exit(1);
  }
}

pid_t cpid, mpid;

int main() {
  if(signal(SIGINT, handler) == SIG_ERR) {
    perror("signal error\n");
    exit(1);	
  }
  mpid = getpid();
  cpid = fork();
  if(cpid < 0) { exit(1); }
  else if(cpid == 0) {
    printf("child process(%d) executes kill function\n", getpid());
    kill(mpid, SIGINT);
    exit(0);
  }
  else {
    printf("main process(%d) is waiting for a signal\n", getpid());
    while(waitpid(cpid,NULL,0) < 0) {
      if(errno != EINTR) {
        exit(1);  
      }
    }
  }
  return 0;
}
