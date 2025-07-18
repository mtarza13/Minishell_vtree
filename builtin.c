#include "executor.h"



int is_builtin(const char *arg)
{
	const char *builin[] = {"cd", "echo" , "export", "unset", "exit","pwd",NULL};
	int i = 0;
	while(builin[i])
	{
		if(strcmp(builin[i],arg) == 0)
		{
			return 1;
		}
		i++;
	}	
	return 0;
}

int exec_cd(char **argv)
{
	 
} 

int exec_builtin(char **argv)
{
	if(strcmp(*argv,"cd") == 0)
		return exec_cd(argv);
	if(strcmp(*argv,"echo") == 0)
		return exec_echo(argv);
	if(strcmp(*argv,"pwd") == 0)
		return exec_pwd(argv);
	if(strcmp(*argv,"unset") == 0)
		return exec_unset(argv);
	if(strcmp(*argv,"export") == 0)
		return exec_export(argv);
	if(strcmp(*argv,"exit") == 0)
		return exec_exit(argv);
	return 1;
}


