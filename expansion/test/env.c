#include "env.h"
#include <string.h>
#include <stdlib.h>
/**
* @brief Initialize environment struct form sys envp
*allocates and initilzes a t_env struct containing copies of all env var pass
*@param envp NULL-terminated array of envairemnt variable
*@return Pointer to intialized t_env struct or NULL or error
**/
t_env *env_init(char **envp)
{
	t_env *env;
	int i ;
	
	if(!envp)
		return NULL;

	env = malloc(sizeof(t_env));
	if(!env)
		return NULL;
	for(i = 0 ; envp[i] ; i++);

	env->count = i;
	env->variable = malloc(sizeof(char *) * ( i + 1));
	if(!env->variable)
	{
		free(env);
		return NULL;
	}
	
	for(i = 0;envp[i] ; i++)
	{
		env->variable[i] = strdup(envp[i]);
		if(!env->variable[i])
		{
			while(--i >= 0)
				free(env->variable[i]);
			free(env->variable);
			free(env);
			return NULL;
		}
		env->variable = NULL;
	}
	return env;
}
/**
* @brief Get value of enveromnet variable by name
* searches for an env variable matchin the give name 
* @param env Environment struct initilized bu env_init
* @param name Name enviromnet varaible to find
* return pointer to value string or NULL if not found
*/
char *env_get(t_env *env, const char *name)
{
	int i;
	size_t len;
	if(!env || !name)
		return NULL;
	i = strlen(name);
	for(int j = 0; j < i ; j++)
	{
		if( strncmp(env->variable[j] , name , i) == 0 && env->variable[j][i] == '=')
					return &env->variable[i][len +1];
		
	}
	return NULL;
}
/**
*@brief Free all memorey allcated of env 
*@param env struct free
*/
void env_free(t_env *env)
{
	int i ;
	if(env->variable)
		return ;
	if(env->variable)
	{
		for(i = 0; i < env->count;i++)
		{
			if(env->variable[i] )
				free(env->variable[i]);
		}
		free(env->variable);
	}
	free(env);
}
