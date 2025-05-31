#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "test.h"

/*
------------------------------------------------------
#################-Algo Step-by-step###################
------------------------------------------------------
1- A function that take string and return 0 or 1
2-Initialze variable:
 + i for index the traver of the string
 + si_quote: flag 1 if we are inside (') or 0.
 + do_quote: flag 1 if we are inside (") or  0.
3- Loop through Input line:
	+ if we find (\) and it's not the last char then:
		$ if inside double quotes or outside all quotes:
			*increment i to skip next character
		$ if we are outside quotes, we still allow basic escaping.
4- Handle quotes:
	+if we see a single quote (') and we are not in double quotes, we toggle the si_quote flag.
	+ if we see double quote ("), and we are not in single quotes, we toggle do_quote.
5- Move to next character
6- Final validation if we ended the loop while still inside quotes, then it return (-1)
to indicat the waiting of closed quotes.
*/

int	valide_quotes(char *line)
{
	int	i;
	int	si_quote;
	int	db_quote;

	i = 0;
	si_quote = 0;
	db_quote = 0;
	while (line[i])
	{
		if (line[i] == '\"' && !si_quote)
			db_quote = !db_quote;
		else if (line[i] == '\'' && !db_quote)
			si_quote = !si_quote;
		i++;
	}
	if (si_quote || db_quote)
		return (-1, printf("ERROR quotes not closed\n"));
	return (1);
}

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>

int	main(void)
{
	char *line;
	char *n_line;
	while (1)
	{
		line = readline(">>>>>");
		if (!line)
			return (1);
		n_line = trim_input(line);
		if (!strcmp(n_line, "exit"))
			break;
		if (!valide_quotes(n_line))
			return (1);
		printf("%s\n", n_line);
		free(n_line);
	}
	free(line);
	free(n_line);
	return (0);
}
