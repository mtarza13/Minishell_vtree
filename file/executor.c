#include "executor.h"
#include <sys/wait.h>
void executor_commands(t_cmd *cmd)
{
	pid_t pid = fork();
	int status ;

	if(pid == -1)
	{
		perror("fork");
		exit(1);
	}
	else if(pid == 0)
	{
		execvp(cmd->argv[0],cmd->argv);
		perror("execvp");
		exit(127);
	}
	else
	{
		waitpid(pid,&status,0);
	}

}
