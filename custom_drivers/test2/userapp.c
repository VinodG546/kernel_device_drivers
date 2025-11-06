#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

#define DEVICE "/dev/chrdev_device"
#define BUF_SIZE 1024


int main()
{
	int fd;
	char write_buf[BUF_SIZE];
	char read_buf[BUF_SIZE];
	ssize_t ret;

	fd=open(DEVICE , O_RDWR);
	if(fd < 0)
	{	
		perror("failed to open device");
		return 1;
	}
	int num1,num2;
	printf("Enter two numbers\n");
	scanf("%d%d",&num1,&num2);
	sprintf(write_buf,"%d %d",num1,num2);
	
	write_buf[strcspn(write_buf, "\n")] = '\0';

    /* Write data to the driver */
    ret = write(fd, write_buf, strlen(write_buf));
    if (ret < 0) {
        perror("Write failed");
        close(fd);
        return 1;
    }

    printf("Written to driver: %s\n", write_buf);

    /* Read data back (converted string) */
    ret = read(fd, read_buf, BUF_SIZE);
    if (ret < 0) {
        perror("Read failed");
        close(fd);
        return 1;
    }

    read_buf[ret] = '\0';
    printf("Read from driver (converted): %s\n", read_buf);

    close(fd);
    return 0;
}
	 
