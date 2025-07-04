#ifndef EXPANSION_H
#define EXPANSION_H

#include "env.h"


int detect_variable(const char *str,int pos);
char *extract_variable(const char *str, int pos , int *end_pos);
char *lookup_variable(t_env *env, const char *var_name, int exit_status);
char *expand_variables(t_env *env ,const char *var_name , int exit_status);

#endif
