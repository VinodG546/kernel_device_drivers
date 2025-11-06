/*userapp.c*/
#include<sys/ioctl.h>
#include<string.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>

#define get_max_min _IOR('a',3,int32_t*)
#define min_max _IOWR('a',1,int32_t*)
#define size _IOW('a',2,int32_t*)
int main()
{
    int fd,num;
    fd = open("/dev/dev_device",O_RDWR);
    if(fd<0){
      printf("open failed\n");
      return -1;
    }
    printf("enter num:");
    scanf("%d",&num);
    int arr[num],i;
    printf("enter the array elements:");
    for(i=0;i<num;i++)
    {
      scanf("%d",arr+i);
    }    
    ioctl(fd,size,&num);
    ioctl(fd,min_max,&arr);
    int mm_arr[2];
    ioctl(fd,get_max_min,&mm_arr);
    printf("max = %d   min = %d\n",mm_arr[0],mm_arr[1]);
}
    
