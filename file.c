//
// Created by klob on 15-6-8.
//

#include "file.h"
void printIntSmap(int pid)
{
    char path_buf [0x100] = {};
    sprintf(path_buf, "/proc/%u/smaps", pid);
    printFile(path_buf);

}
void printFile(char * path )
{
    FILE *fp = fopen(path, "rb");
    char str[1024];
    while(!feof(fp)){
        fgets(str,1024,fp);
        printf("%s",str);
    }
}
void getSystemInfo(char cmd[] ) {
    system(cmd);
}
void getSystemInfo2(char cmd[] ) {
    FILE * fp;
    char buffer[80];
    fp=popen(cmd,"r");      //type的参数只能是“r”或"w"
    while (fgets(buffer,sizeof(buffer),fp))
    {
        printf("%s",buffer);
    }
    pclose(fp);
}

int global_init_a=1;
int global_uninit_a;
static int static_global_init_a=1;
static int static_global_uninit_a;
const int const_global_a=1;

int global_init_b=1;
int global_uninit_b;
static int static_global_init_b=1;
static int static_global_uninit_b;
const int const_global_b=1;



void fileTest() {


    int local_init_a=1;
    int local_uninit_a;
    static int static_local_init_a=1;
    static int static_local_uninit_a;
    const int const_local_a=1;

    int local_init_b=1;
    int local_uninit_b;
    static int static_local_init_b=1;
    static int static_local_uninit_b;
    const int const_local_b=1;

    int * malloc_p_a;
    malloc_p_a=malloc(sizeof(int));

    printf("\n         &global_init_a=%p \tglobal_init_a=%d\n",&global_init_a,global_init_a);

    printf("       &global_uninit_a=%p \tglobal_uninit_a=%d\n",&global_uninit_a,global_uninit_a);

    printf("  &static_global_init_a=%p \tstatic_global_init_a=%d\n",&static_global_init_a,static_global_init_a);

    printf("&static_global_uninit_a=%p \tstatic_global_uninit_a=%d\n",&static_global_uninit_a,static_global_uninit_a);

    printf("        &const_global_a=%p \tconst_global_a=%d\n",&const_global_a,const_global_a);


    printf("\n         &global_init_b=%p \tglobal_init_b=%d\n",&global_init_b,global_init_b);

    printf("       &global_uninit_b=%p \tglobal_uninit_b=%d\n",&global_uninit_b,global_uninit_b);

    printf("  &static_global_init_b=%p \tstatic_global_init_b=%d\n",&static_global_init_b,static_global_init_b);

    printf("&static_global_uninit_b=%p \tstatic_global_uninit_b=%d\n",&static_global_uninit_b,static_global_uninit_b);

    printf("        &const_global_b=%p \tconst_global_b=%d\n",&const_global_b,const_global_b);



    printf("\n          &local_init_a=%p \tlocal_init_a=%d\n",&local_init_a,local_init_a);

    printf("        &local_uninit_a=%p \tlocal_uninit_a=%d\n",&local_uninit_a,local_uninit_a);

    printf("   &static_local_init_a=%p \tstatic_local_init_a=%d\n",&static_local_init_a,static_local_init_a);

    printf(" &static_local_uninit_a=%p \tstatic_local_uninit_a=%d\n",&static_local_uninit_a,static_local_uninit_a);

    printf("         &const_local_a=%p \tconst_local_a=%d\n",&const_local_a,const_local_a);


    printf("\n          &local_init_b=%p \tlocal_init_b=%d\n",&local_init_b,local_init_b);

    printf("        &local_uninit_b=%p \tlocal_uninit_b=%d\n",&local_uninit_b,local_uninit_b);

    printf("   &static_local_init_b=%p \tstatic_local_init_b=%d\n",&static_local_init_b,static_local_init_b);

    printf(" &static_local_uninit_b=%p \tstatic_local_uninit_b=%d\n",&static_local_uninit_b,static_local_uninit_b);

    printf("         &const_local_b=%p \tconst_local_b=%d\n",&const_local_b,const_local_b);


    printf("             malloc_p_a=%p \t *malloc_p_a=%d\n",malloc_p_a,*malloc_p_a);
}
