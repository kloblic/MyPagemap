#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>

#define PM_ENTRY_BYTES sizeof(uint64_t)
#define GET_BIT(X,Y) (X & ((uint64_t)1<<Y)) >> Y
#define GET_PFN(X) X & 0x7FFFFFFFFFFFFF
#define    PFN_MASK          ((((uint64_t)1)<<55)-1)
#define    PFN_PRESENT_FLAG  (((uint64_t)1)<<63)

const int __endian_bit = 1;
// 小端模式 Little-Endian ：低位字节排放在内存的低地址端，高位字节排放在内存的高地址端。
// 大端模式 Big-Endian ：   高位字节排放在内存的低地址端，低位字节排放在内存的高地址端。

// 是否是大端模式  0 ：否  1 ：是
#define is_bigendian() ( (*(char*)&__endian_bit) == 0 )

int i, c, pid, status;
unsigned long virt_addr;
uint64_t read_val, file_offset;
char path_buf [0x100] = {};
FILE * f;
char *end;
char * argv[]={"","9767","00602000"};
char    page_map_file [0x100]= {};
int read_pagemap(char * path_buf, unsigned long virt_addr);
int mem_addr_vir2phy(unsigned long vir);

int main(){
    test();
    //printf("%lu\n", GET_BIT(0xA680000000000000, 63));
    //return 0;
    int argc=3;
    if(argc!=3){
        printf("Argument number is not correct!\n pagemap PID VIRTUAL_ADDRESS\n");
        return -1;
    }
    if(!memcmp(argv[1],"self",sizeof("self"))){
        sprintf(path_buf, "/proc/self/pagemap");
        pid = -1;
    }
    else{
        pid = strtol(argv[1],&end, 10);
        if (end == argv[1] || *end != '\0' || pid<=0){
            printf("PID must be a positive number or 'self'\n");
            return -1;
        }
    }
    //虚拟地址
    virt_addr = strtol(argv[2], NULL, 16);
    if(pid!=-1)
    {
        //根据pid得到进程地址映射文件名
        sprintf(path_buf, "/proc/%u/pagemap", pid);
    }

    read_pagemap(path_buf, virt_addr);
    mem_addr_vir2phy(virt_addr);
    return 0;
}


int read_pagemap(char * path_buf, unsigned long virt_addr){
    printf("是否是大端模式 %d\n", is_bigendian());
    f = fopen(path_buf, "rb");
    if(!f){
        printf("不能打开文件 %s\n", path_buf);
        return -1;
    }

    //Shifting by virt-addr-offset number of bytes
    //and multiplying by the size of an address (the size of an entry in pagemap file)
    //得到文件偏移
    file_offset = virt_addr / getpagesize() * PM_ENTRY_BYTES;
    printf("虚拟地址: 0x%lx, 页大小: %d, Entry_size: %d\n", virt_addr, getpagesize(), PM_ENTRY_BYTES);
    printf("读取文件 %s at 0x%llx\n", path_buf, (unsigned long long) file_offset);
    status = fseek(f, file_offset, SEEK_SET);
    if(status){
        perror("未找到");
        return -1;
    }
    errno = 0;
    read_val = 0;
    unsigned char c_buf[PM_ENTRY_BYTES];
    for(i=0; i < PM_ENTRY_BYTES; i++){
        c = getc(f);
        if(c==EOF){
            printf("\n到达文件尾\n");
            return 0;
        }
        if(is_bigendian())
            c_buf[i] = c;
        else
            c_buf[PM_ENTRY_BYTES - i - 1] = c;
        printf("[%d]0x%x ", i, c);
    }
    for(i=0; i < PM_ENTRY_BYTES; i++){
        //printf("%d ",c_buf[i]);
        read_val = (read_val << 8) + c_buf[i];
    }
    printf("\n");
    printf("实际物理地址: 0x%llx\n", (unsigned long long) read_val);
    //if(GET_BIT(read_val, 63))
    if(GET_BIT(read_val, 63))
        printf("物理页框号: 0x%llx\n",(unsigned long long) GET_PFN(read_val));
    else
        printf("页不存在\n");
    if(GET_BIT(read_val, 62))
        printf("页被换出\n");
    fclose(f);
    return 0;
}

int mem_addr_vir2phy(unsigned long vir) {

    unsigned long *phy;
    int  a=*(int *)(void *)vir;
    int fd;
    int page_size=getpagesize();
    unsigned long vir_page_idx = vir/page_size;
    unsigned long pfn_item_offset = vir_page_idx*sizeof(uint64_t);
    uint64_t pfn_item;
    sprintf(page_map_file, "/proc/%u/pagemap", pid);
    fd = open(page_map_file, O_RDONLY);
    if (fd<0)
    {
        printf("error 1");
        printf("open %s failed", page_map_file);
        return -1;
    }


    if ((off_t)-1 == lseek(fd, pfn_item_offset, SEEK_SET))
    {
        printf("error 2");
        printf("lseek %s failed", page_map_file);
        return -1;
    }


    if (sizeof(uint64_t) != read(fd, &pfn_item, sizeof(uint64_t)))
    {
        printf("error 3");
        printf("read %s failed", page_map_file);
        return -1;
    }


    if (0==(pfn_item & PFN_PRESENT_FLAG))
    {
        printf("error 4");
        printf("page is not present");
        return -1;
    }

    printf("%p\n",(pfn_item & PFN_MASK)*page_size);
    printf("%p\n",vir % page_size);
    phy = (pfn_item & PFN_MASK)*page_size + vir % page_size;
    printf("%p\n",phy);

    return 0;
}


/*
 * 0x00007f347dc63810
 * 0x86000000000540a0
 *
 * 16进制查看
 * od -tx1 -tc -Ax
 *
 * // getSystemInfo2("ls /proc/");
    //cat status
    //cat mapsce
    //cat cmdline
    //smaps
   //global();
 *
 *
 *
 * */