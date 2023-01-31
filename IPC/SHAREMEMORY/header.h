#ifndef _HEADER_H
#define _HEADER_H
#define MAX_SIZE 2048

typedef struct _shared_data {
  bool write;
  char text[MAX_SIZE];
} shared_data;

typedef struct _msgform {
  long mtype;
  char mtext;
} msgform;

typedef union _semum {
  int val;
  struct semid_ds *buf;
  unsigned char *arr; 
} semun;

#endif
