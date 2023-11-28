#include<linux/kernel.h>
#include<sys/syscall.h>
#include<unistd.h>
#include<stdio.h>
int main(int argc,char** argv){
	int nRet1 = syscall(548,20,18);
	printf("%d\n",nRet1);
	int nRet2 = syscall(549,4,2,9);
	printf("%d\n",nRet2);
	return 0;
}
