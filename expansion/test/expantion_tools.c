#include "expansion.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/**
* @brief Detects if position points to avalid variable reference
*
* @param str Input string to check 
* @patam pos postision in the string to check for variable reference
* @return int 1 
*/
int detect_variable(const char  *str , int pos)
{

	if(!str || pos < 0)
		return 0;
	if(str[pos + 1] == '$')
		return 0;
	
	if(str[pos -1 ] == '\\')
		return 0;

	if(str[pos + 1] == '?')
		return 1;
	
	if(str[pos +1 ] == '{')
		return 0;

	if(str[pos + 1] == '\0')
	{
		return 0;
	}
 	if(isalpha(str[pos + 1]) || str[pos + 1] == '_')
        return 1;
	return 0;
}
/**
* @brief Etracrs variable name form string 
* @param str the input string countaing a vriable refrene
* @param pos the position of the position of the @character 
* @return char* the variable name (without $), must be freed by caller
*/
char *extract_var_name(const char *str, int pos)
{
    int start, end;
    char *name;
    
    start = pos + 1;
    if (str[start] == '?') {
        name = malloc(2);
        name[0] = '?';
        name[1] = '\0';
        return name;
    }
    
    if (isalpha(str[start]) || str[start] == '_') {
        end = start;
        
        while (str[end] && (isalnum(str[end]) || str[end] == '_'))
            end++;
            
        name = malloc(end - start + 1);
        strncpy(name, str + start, end - start);
        name[end - start] = '\0';
        return name;
    }
    return strdup("");
}
/**
* @brief look up and return value of variable
*
* @param env envaironment 
* @param var_name of the variable to lockup
* @param exit_status Current exit status (for ?)
* @return char *value of the variable (must br freed by caller)
*/

char  *lookup_variable(t_env *env , const char *var_name, int exit_status)
{
	char exit_status_str[12];

	if(!var_name)
		return NULL;

	if(strcmp(var_name , "?" ) == 0)
	{
		sprintf(exit_status_str , "%d", exit_status);
			return strdup(exit_status_str);
	}
	if(env)
	{
		char *value = env_get(env,var_name);
		if(value)
				return strdup(value);
	}
	return strdup("");
}





















