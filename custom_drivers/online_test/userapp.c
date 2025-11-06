/*userapp.c*/

#include<string.h>
#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>


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
    /*char str[500]='\0';
    for(i=0;i<num;i++)
    {
      sprintf(str , "%s %d",str,arr[i]);
    }*/
    
    write(fd,arr,num*sizeof(int));
    int max_min[2];
    read(fd,max_min,sizeof(max_min));
    printf("max = %d   min = %d\n",max_min[0],max_min[1]);
}
    
