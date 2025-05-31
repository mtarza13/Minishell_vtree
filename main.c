#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <strings.h>
#include "token.h"
/*
==============================================================================
 main.c - Entry point for minishell lexer test
==============================================================================

 Description:
 This program reads a single line of input from the user using readline(),
 then passes that line to the lexer function to tokenize and display the
 tokens.

 Usage:
 Run the executable, enter a shell command line, and see tokenized output.

 Notes:
 - Uses GNU readline for user input.
 - Does not handle multiple lines or shell execution yet.
 - Memory allocated by readline is not freed here (consider free(line)).

==============================================================================
*/

int main()
{
	char *line = readline("-->");
	lexer(line);
	free(line);
}
