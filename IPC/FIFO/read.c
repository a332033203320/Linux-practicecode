#include<stdio.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>

int main(int argc, char** argv) {
  int fd;
  char buff[1024];
  if(mkfifo("fifo1", 0666) < 0 && errno != EEXIST) { 
    printf("Create fifo error\n");
    exit(1);
  }
  fd = open("fifo1", O_RDONLY);
  if(fd < 0) { 
    printf("Open fifo error\n");
    exit(1);
  }
  printf("Read start...\n");
  while(read(fd, buff, 1024) > 0) { 
    printf("Read message: %s", buff); 
    sleep(1);
  }
  close(fd);
  return 0;
}
