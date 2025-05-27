#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
	int fd = open("miss.txt",O_RDONLY);
	if(fd < 0)	
	{
		printf("%s",strerror(errno));
		perror("open");
	}
}
