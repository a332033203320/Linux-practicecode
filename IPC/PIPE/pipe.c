#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<wait.h>
#include<errno.h>

int main() {
  int fd[2], status;
  pid_t pid;
  char buff[20];
  if(pipe(fd) < 0) { return -1; }
  pid = fork();  
  if(pid == 0) {
    close(fd[0]);
    write(fd[1],"hello world\n",13);
  }
  else if(pid < 0) { printf("Fork error\n"); return -1; }
  else {
    while(waitpid(pid,&status,0) < 0) {
      if(errno == EINTR) { printf("Something error\n"); return -1; }
    }   
    close(fd[1]);
    read(fd[0],buff,20);
    printf("Recevie message: %s", buff);
  }
  return 0;
}
