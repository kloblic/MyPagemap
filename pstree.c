#include "pstree.h"
int my_getpid(char *str){
    int len=strlen(str);
    char num[10];
    int i,j,ret;
    if(strncmp(str,"Pid",3)==0){
        //printf("%s",str);
        for(i=0;i<len;i++){
            if(str[i]>='0'&&str[i]<='9')
                break;
        }
        //printf("len=%d,i=%d\n",len,i);
        for(j=0;j<len-i;j++){
            num[j]=str[i+j];
        }
        //num[j]='\0';
        //printf("%s",num);
        ret=atoi(num);
        //printf("ret=%d\n",ret);
    }
    else ret=0;
    return ret;
}
int my_getppid(char *str){
    int len=strlen(str);
    char num[10];
    int i,j,ret;
    if(strncmp(str,"PPid",4)==0){
        //printf("%s",str);
        for(i=0;i<len;i++){
            if(str[i]>='0'&&str[i]<='9')
                break;
        }
        //printf("len=%d,i=%d\n",len,i);
        for(j=0;j<len-i;j++){
            num[j]=str[i+j];
        }
        //num[j]='\0';
        //printf("%s",num);
        ret=atoi(num);
        //printf("ret=%d\n",ret);
    }
    else ret=0;
    return ret;
}
char * my_getName(char *str) {
    int j,k;
    char name[1024];
    if(strncmp(str,"Name",4)==0){
        for(j=4;j<strlen(str);j++){
            if(str[j]>='A'&&str[j]<='z')
                break;
        }
        for(k=j;k<strlen(str);k++){
            name[k-j]=str[k];
        }
        name[k-j-1]='\0';
    }
    return name;
}
int child_exist(info *file,int count,int ppid){
    int i;
    for(i=0;i<count;i++){
        if(file[i].flag==0&&file[i].ppid==ppid)
            return 1;
    }
    return 0;
}
void print_pstree(info *file,int count,int ppid,int rec){
    int i,j,k;

    for(i=0;i<count;i++){

        if(file[i].flag==0&&file[i].ppid==ppid){
            file[i].rec=rec+1;
            file[i].flag=1;
            for(k=0;k<rec;k++)
                printf("  ");
            printf("%s\n",file[i].name);
            //if(child_exist(file,count,file[i].pid)==1){
            //	printf("---",file[i].pid);
            //}
            print_pstree(file,count,file[i].pid,file[i].rec);
        }

    }
}
int treeTest(){
    int i,j,k,total,s1,s2,count,t;
    char str[1024],dir[1024];
    struct dirent **namelist;
    strcpy(dir,default_path);
    total = scandir(dir, &namelist, 0, alphasort);
    printf("path=%s,total=%d\n",dir,total);
    for(i=0;i<total;i++){
        strcpy(str,namelist[i]->d_name);
        if(str[0]>='0'&&str[0]<='9')
            count++;
    }
    printf("进程数:%d\n",count);
    info file[1024];
    i=0;t=0;
    while(i<total){
        FILE *fp;
        char path[1024],name[1024];
        int pid,ppid;
        strcpy(str,namelist[i]->d_name);
        strcpy(path,default_path);
        //printf("%s\n",str);
        if(str[0]>='0'&&str[0]<='9'){
            strcat(path,str);
            strcat(path,"/status");
            fp=fopen(path,"r");
            while(!feof(fp)){
                fgets(str,1024,fp);
//pid
                if((s1=my_getpid(str))!=0)
                    pid=s1;
//ppid
                if((s2=my_getppid(str))!=0)
                    ppid=s2;
//name
                strcpy(file[t].name,my_getName(str));

                file[t].pid=pid;
                file[t].ppid=ppid;

            }
            fclose(fp);
            printf("%d,%s,pid=%d,ppid=%d,name=%s#\n",t,path,file[t].pid,file[t].ppid,file[t].name);
            t++;
        }
        i++;
    }
    memset(&file->flag,0,count);
    memset(&file->rec,0,count);
    print_pstree(file,count,0,0);
    while (1){;}
}

