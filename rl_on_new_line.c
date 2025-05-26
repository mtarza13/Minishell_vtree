#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>
#include <unistd.h>


void sig_handler(int sig)
{
	(void)sig;
	write(STDOUT_FILENO,"\n",2);
	rl_on_new_line();
    rl_replace_line("exit", 0);
	rl_redisplay();
}

int main()
{
	signal(SIGINT,sig_handler);

	char *line;

	while((line = readline("#")))
	{
		if(*line)
			add_history(line);
		free(line);
	}
}
