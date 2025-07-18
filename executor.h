#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <string.h>
#include "ast.h"
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>


int exec_ast(t_ast_node *node);
int is_builtin(const char *arg);
int exec_builtin(char **arg);
#endif 
