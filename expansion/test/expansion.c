#include "expansion.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
	* @brief Helper function to calculate lenght of expand string
*
* @param env environment struct 
* @param str string calculter exapanded length for 
* @param exit_status exitiot status current exit status
* @return siz_t length of expanded string
*/
size_t  calculate_expanded_length(t_env *env , const char *str, int exit_status)
{
	size_t len = 0;
	int i = 0;

	while(str[i])
	{
		if(str[i] == '$' && detect_variable(str,i))
		{
			int end_pos;
			char *var_name = extract_variable(str,i,&end_pos);
			if(var_name)
			{
				char *value = lookup_variable(env,var_name,exit_status);
				if(value)
				{
					len += strlen(value);
					free(value);
				}
				free(var_name);
				i = end_pos;
			}
			else 
				i++, len++;
		}
		else
			i++,len++;
	}
	return len;
}
/**
* @brief Exepand all variable in string 
*
* @param env Envirenment struct 
* @param str string to expand
*/
char *expad_variables(t_env *env , const char *str,int exit_status)
{
	char *result;
	size_t result_len;
	int i = 0, j = 0;

	if(!str)
		return NULL;

	result_len = calculate_expanded_length(env,str,exit_status);
	result = malloc(result_len +1);
	if(!result)
		return NULL;

	while(str[i])
	{
		if(str[i] && detect_variable(str,i))
		{
			int end_pos ;
			char *var_name = extract_variable(str,i,&end_pos);
			if(var_name)
			{
				char *value = lookup_variable(env,var_name,exit_status);
				if(value)
				{
					strcpy(&result[i],value);
						j += strlen(value);
					free(value);
				}
				free(var_name);
				i = end_pos;
			}
			else
				result[j++] = str[i++];
		}
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return result ;
}
