#include<stdio.h>
#include<limits.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include"ioc.h"

int main(int argc, char* argv[]) {
  if(argc < 2) { printf("Usage: /dev/mydev\n"); goto failed; }
  printf("%s\n", argv[1]);
  int fd = open(argv[1], O_RDWR), ret = INT_MIN;
  char rbuff[64], wbuff[4] = { 'a', 'b', 'c', 'd' }; 
 
  if(fd < 0) { printf("Open failed\n"); goto failed; } 
  read(fd,rbuff,sizeof(rbuff));  
  printf("buffer: %s\n",rbuff); 
  write(fd,wbuff,sizeof(wbuff));
 
  if(ioctl(fd,GETNUM,&ret) < 0) { printf("Get num failed\n"); goto failed; }
  printf("Get num: %d\n", ret); //ret = 0
  ret = 10; //kernel val = 10;
  printf("Set kernel val: %d\n",ret);
  if(ioctl(fd,SETNUM,&ret) < 0) { printf("Set num failed\n"); goto failed; }
  ret = 100;
  if(ioctl(fd,XNUM,&ret) < 0) { printf("Exchange num failed\n"); goto failed; } 
  printf("Get exchanged num: %d\n", ret); //ret = 10, kernel val = 100
  ret = close(fd);
  if(ret < 0) { printf("Close file failed\n"); goto failed; }
  return 0;
failed:
  return EXIT_FAILURE;
}
