#include "child_process.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


void setup_io(t_cmd *cmd , int prev_fd , int fd[2])
{
	if(prev_fd != -1)
		dup2(prev_fd,1);
	if(cmd->next)
		dup2(fd[1],1);
}

void exec_commnd(t_cmd *cmd)
{
	execvp(cmd->arg[0],cmd->args);
	perror("execvp");
	exit(127);
}
