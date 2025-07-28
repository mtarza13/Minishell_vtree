
#include "../include/minishell.h"

t_token *creat_token(t_token_type type, char *value)
{
	t_token *new;
	new = malloc(sizeof(t_token));
	new->type = type;
	new->value = strdup(value);
	new->next = NULL;
	return new;
}
void add_token(t_token **token ,t_token *new_token)
{
	if(!token || !new_token)	{
		return;
	}

	if(*token == NULL)
	{
		*token = new_token; 
		return;
	}
	t_token *current = *token;
		while(current->next)
			current = current->next;
		current->next = new_token;
	return;
}
static int check_input(char *input, int *i, t_token **token)
{
	if(input[*i] == '|')
	{
		add_token(token,creat_token(TOKEN_PIPE,"|"));
		(*i)++;
		return(1);
	}

	if(input[*i] == '<')
	{
		if(input[*i + 1] == '<')
		{
		add_token(token,creat_token(TOKEN_REDIR_HERDOC,"<<"));
		*i+= 2;
		}
		else{
		add_token(token,creat_token(TOKEN_REDIR_IN,"<"));
		(*i)++;
		}
		return(1);
	}
	if(input[*i] == '>')
	{
		if(input[*i + 1] == '>')
		{
		add_token(token,creat_token(TOKEN_REDIR_APPEND,">>"));
		*i+= 2;
		}
		else{
		add_token(token,creat_token(TOKEN_REDIR_OUT,">"));
		(*i)++;
		}
		return(1);
	}

	return(0);
}

void free_token(t_token *list)
{
	t_token *tmp;
	while(list)
	{
		tmp = list;
		list = list->next;
		free(tmp->value);
		free(tmp);
	}
}
int check_quote(char *input)
{
	int i = 0;
	int dq , sq = 0;
	while(input[i])
	{
	}
	if(!sq || !dq)
	{
		printf("syntax erro unclose quote");
		return 1;
	}
return 0;
}
t_token *tokenaizer(char *input)
{
	int i ;
    i = 0;
	t_token *token ;
	token = NULL;
	int start;
	while(input[i])
	{ 
		while(input[i] && (input[i] == ' ' || input[i] == '\t'))
				i++;
		if(!input[i])
			break;
		if(check_input(input,&i,&token))
			continue;

		start = i;
	while (input[i] && input[i] != ' ' && input[i] != '<' &&
			   input[i] != '>' && input[i] != '\t' )
	{
			i++;
	}

		if(i > start ){
		char *word = ft_substr(input,start,i - start);
		add_token(&token,creat_token(TOKEN_WORD,word));
		free(word);
		}
		printf("dkhl\n");
		
	} 
	return(token);
	
}
