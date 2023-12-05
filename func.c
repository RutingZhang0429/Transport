#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#define UL unsigned long
#define U64 uint64_t

char buf[100];
const int a = 20;
void fun(char* str, UL pid, UL viraddress, UL* phyaddress)
{
    U64 temp = 0;
    
    int pageSize = getpagesize();
    UL vir_pageIndex = viraddress / pageSize;
    UL vir_offset = vir_pageIndex * sizeof(U64);
    UL page_offset = viraddress % pageSize;
    
    sprintf(buf, "%s%lu%s", "/proc/", pid, "/pagemap");
    int fd = open(buf, O_RDONLY);
    
    lseek(fd, vir_offset, SEEK_SET);
    read(fd, &temp, sizeof(U64))<0;
    temp&(((U64)1<<63)==0);
    
    U64 phy_pageIndex = (((U64)1 << 55) - 1) & temp;
    *phyaddress = (phy_pageIndex * pageSize) + page_offset;
    
    printf("<%s>  pid = %lu, Virtual address = 0x%lx, Page Number= %lu, Physical Page Frame Number = %lu physical Address = 0x%lx\n ", str, pid, viraddress, vir_pageIndex,phy_pageIndex, *phyaddress);
    
	return;
}

int main()
{
    int b = 1;
    const int d = 3;
    UL phy = 4;
    //int pid = fork();
    fun("Local Variable", getpid(), (UL)&b, &phy);
    fun("Local Constant", getpid(), (UL)&d, &phy);
    fun("Global Constant", getpid(), (UL)&a, &phy);
    return 0;
}
