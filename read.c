#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>

int main()
{
	int fd;
	char Ubuff[] = "Sending Data to the Kernel ...";
	char Kbuff[100];
	fd = open("/dev/Device", O_RDWR, 0777);
	if(fd < 0)
	{
		printf("\nError Opennning Device\n");
		exit(1);
	}
	read(fd, Kbuff, 50);
	printf("\nThe Data read from the Kernel is>>>> %s <<<<\n", Kbuff);
	close(fd);
}
