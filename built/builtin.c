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

int set_env(const char *name , const char *value)
{
	if(setenv(name,value,1) != 0)
	{
		perror("setenv");
		return 1; }
	return 0;
}
int exec_cd(char **argv)
{
	char cwd[2024];
	if(argv[1] && argv[2])
	{
		fprintf(stderr,"cd: too many agruments\n");
		return 1;
	}

	char *path = argv[1];
	if(getcwd(cwd,sizeof(cwd)) == NULL)
	{
		perror("cd getwcd");
		return 1; 
	}
	char *oldpwd = strdup(cwd);
	char *target = NULL;
	if(!argv[1])
	{
		target = getenv("HOME");
		if(!target)
		{
			fprintf(stderr,"cd : HOME not set\n");
			free(oldpwd);
			return 1;
		}
		else if(strcmp(argv[1] , "-") == 0)
		{
			target = getenv(oldpwd);
			return 1;
		}
		printf("%s\n", path);
	}else
		target = argv[1];
	
	if(chdir(target)!=0)
	{
		if(access(target,F_OK) != 0)
		{
			fprintf(stderr,"cd: no such file or directory: %s\n", target);
			return 1;
		}
		else
			perror("cd");
		free(oldpwd);
		return 1;
	}
	if(getcwd(cwd,sizeof(cwd)))
	{
	if(set_env("PWD",cwd) != 0)
			return 1;
	}
	if(set_env("OLDPWD",oldpwd) != 0)
		return 1;
	free(oldpwd);
 	return 0;
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


