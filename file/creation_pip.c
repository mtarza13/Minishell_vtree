#include "executor.h"
#include <unistd.h>


void creat_pipe(t_cmd *cmd , int count)
{
	int fd[2];
	pipe(fd);
	int i = 0;
	while(i < count)
	{	
		pid_t pid = fork();
		if(pid == 0)
		{
			dup2(fd[1],STDOUT_FILENO);

		}
	}
}
