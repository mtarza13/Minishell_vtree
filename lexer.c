#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <crypt.h>
#include <string.h>  // Added missing include
#include <ctype.h>   // Added missing include
#include "token.h"

/*
==============================================================================
 Lexer Module - Tokenization Logic
==============================================================================
*/

tokentype analyze_line(char *word)
{
	if(strcmp(word ,"|") == 0) 
		return token_pip;
	else if(strcmp(word ,"<") == 0)
		return token_redir_in;
	else if(strcmp(word ,">") == 0)
		return token_redir_out;
	else if(strcmp(word ,">>") == 0)
		return token_append;
	else if(strcmp(word ,"<<") == 0) 
		return token_herdoc;
	else if(strcmp(word ,"'") == 0) 
		return token_single_quote;
	else if(strcmp(word ,"\"") == 0) 
		return token_double_quote;

	return token_word;
}

Token *lexer(char *line)
{
	int i = 0;
	int start = 0;
	Token *head = NULL;

	while(line[i]) 
	{
		if(isspace(line[i]))
		{
			if(i > start)
			{
				char *word = strndup(&line[start], i - start);
				tokentype type = analyze_line(word);
				Token *token_new = creat_token(word,type);
				add_token(&head,token_new);
				free(word);
			}
			while(isspace(line[i]))i++;
			start = i;
		}else
			i++;
	}
	if(i > start)
	{
		char *word = strndup(&line[start], i - start);	
		tokentype type = analyze_line(word);
		Token *token_new = creat_token(word,type);
		add_token(&head,token_new);
		free(word);
	}
	
	// DON'T free the tokens here - the caller needs them!
	// free_token(head);  // ← REMOVED THIS LINE
	return head; 
}
