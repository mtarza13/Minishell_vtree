#include <unistd.h>
#include <stdio.h>


int main()
{
	int fd[2];
	char buffer[100];

	write(fd[1],"hello",5);
	read(fd[0],buffer,5);

	buffer[5] = '\0';
	printf("form pipe %s \n",buffer);

}
