#ifndef ENV_H
#define ENV_H


typedef struct s_env
{
	char **variable;
	int count;
}t_env;




t_env *env_init(char **envp);
char *env_get(t_env *env , const char *name );
void env_free(t_env *env);

#endif
