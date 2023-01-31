#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<time.h>
#include<string.h>

int main(int argc, char** argv) {
  int fd;
  char buff[256];
  printf("Process id: %d\n", getpid());
  fd = open("fifo1", O_WRONLY | O_CREAT, S_IRWXU);
  if(fd < 0) { printf("Create fifo file error\n"); exit(1); }
  for(int i = 0; i < 5; i++) {
    time_t curr;
    time(&curr);
    int sz = sprintf(buff,"Process %d's times is %s",getpid(),ctime(&curr));
    printf("%s",buff);
    if(write(fd,buff,sz) < 0) {
      printf("Write fifo file error\n");
      close(fd);
      exit(1);
    }
    sleep(1);
  }
  close(fd);
  return 0;
}
