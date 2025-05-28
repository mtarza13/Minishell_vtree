#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdlib.h>
#include <strings.h>

typedef struct d_f
{
	char *data;
	struct d_f *next;
}l_f;

void	creat_lit(l_f **head ,char *s)
{
	l_f *new = malloc(sizeof(l_f));

	new->data = s;
	new->next = *head;
	
	*head = new;
}
l_f *head = NULL;

void add_to_list(char *s)
{	
	creat_lit(&head, s);
}
int main()
{
	char *s;

	while(1) {
		s = readline(">> "); 
	//add_to_list(s);
   add_history(s);
	}

	while(head)
	{
		printf("%s->",head->data);
		head = head->next;
	}
	printf("amin");
}

