

void handle_input_redirection(t_cmd *cmd )
{
	if(cmd->infile)
	{
		int fd = open(cmd->infile,O_RDONLY);
		if(fd < 0)
		{
			perror("infile open faild");
			exit(1);
		}
		dup2(fd,0);
		close(fd);
	}
}


void handle_output_ridairaction(t_cmd *cmd)
{
	if(cmd->outfile)
	{
		int flag = O_CREAT | O_WRONLY;
		if(cmd->append)
			flag |= O_APPEND;
		else 
			falgs | O_TRUNC;

		int fd = open(cmd->outfile , flags, 0644);
		if(fd < 0)
		{
			perror("outfile open failed");
			exit(1);
		}
		dup2(fd,STDOUT_FILENO);
		close(fd);
	}
}

void 
