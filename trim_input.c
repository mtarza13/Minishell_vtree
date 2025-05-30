#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/**
 * ft_isspace - check if charcter are space belongs
 * @c: charcter to check.
 * Return: 1 if is space otherwise 0.
 */
int	ft_isspace(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ' )
	       return (1);
	return (0);
}

/**
 * ft_strlen - count string length.
 * @str: The string.
 * Return: length
 */
size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

/**
 * trim_input - trims extra spaces and tabs
 * @str: The string.
 * Return: New allocated clear string.
 */
char	*trim_input(char *str)
{
	size_t	start;
	size_t	end;
	char	*n_str;
	size_t	i;

	start = 0;
	i = 0;
	if (!str)
		return (NULL);
	while (str[start] && ft_isspace(str[start]))
		start++;
	end = ft_strlen(str);
	while (end > start && ft_isspace(str[end - 1]))
		end--;
	n_str = malloc(end - start + 1);
	printf("start =%zu\n end = %zu\n n_size= %zu\n", start, end, (end - start));
	if (!n_str)
		return (NULL);
	while (start < end)
	{
		if (str[start] == '\t')
			n_str[i] = ' ';
		else
			n_str[i] = str[start];
		i++;
		start++;
	}
	n_str[i] = '\0';
	return (n_str);
}

#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
	char	*line;
	char	*n_line;

	while (1)
	{
		line = readline(">>> ");
		if (strcmp(line, "exit") == 0)
			break;
		n_line = trim_input(line);
		free(line);
		printf("n_line:|%s|\n", n_line);
		free(n_line);
	}
	free(line);
	return (0);
}
