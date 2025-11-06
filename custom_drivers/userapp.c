#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>
//#define device /dev/mydev_device
int main()
{
    int fd=0;
    char ip_str[100];
    char op_str[100];
    fd=open("/dev/mydev_device",O_RDWR);
    while(1)
    {
    	printf("enter msg:");
    	fgets(ip_str,sizeof(ip_str),stdin);
    	write(fd,ip_str,strlen(ip_str));
    	read(fd,op_str,sizeof(op_str));
    	printf("%s\n",op_str);
    }
}
