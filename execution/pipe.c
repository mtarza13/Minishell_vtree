#include "pipe.h"
#include "child_process.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void create_pipe(t_cmd *cmd_list)
{
	int prev_fds = -1;
	int fd[2];
	t_cmd *cmd = cmd_list;

	while(cmd)
	{
		if(cmd->next && pipe(fd) == -1)
			exit(1);
		pid_t pid = fork();
		if(pid == -1)
			exit(1);
		if(pid == 0)
		{
			setup_io(cmd , prev_fds,fd);
			exec_command(cmd);
			exit();
		}
		if(cmd->next)
		{
			close(fd[1]);
			prev_fd = fd[0];
		}
		cmd = cmd->next;
	}
	while(wait(NULL) > 0);
}
