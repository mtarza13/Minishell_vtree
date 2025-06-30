#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>



int main()
{
	pid_t pid = fork();
	int status;
	while(1)
	{
	if(pid == 0)
	{
		printf("this is child");
	}
	else
	{
		waitpid(pid , &status,0);
		printf("Exit code %d",WEXITSTATUS(status));
	}
	}
}
