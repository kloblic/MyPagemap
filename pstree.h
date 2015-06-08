#ifndef MYPAGEMAP_PSTREE_H
#define MYPAGEMAP_PSTREE_H

#include<stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
char default_path[1024]="/proc/";
int s=0;
typedef struct file_info{
    int pid;
    int ppid;
    char name[1024];
    int flag;
    int rec;
}info;

extern int my_getpid(char *str);
extern int my_getppid(char *str);
extern char * my_getName(char *str);
extern int child_exist(info *file,int count,int ppid);
extern void print_pstree(info *file,int count,int ppid,int rec);
extern int treeTest();

#endif //MYPAGEMAP_PSTREE_H
