#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <crypt.h>
#include "token.h"

/*
==============================================================================
 Lexer Module - Tokenization Logic
==============================================================================

 Description:
 This module provides basic lexical analysis functionality. It reads a line
 of shell input, splits it into meaningful tokens (commands, operators, etc.),
 and classifies each token based on its type.

 Current Features:
 - Token recognition: |, <, >, >>, <<, ' and "
 - Word splitting based on whitespace
 - Dynamic creation of a linked list of tokens

 Limitations (To be addressed):
 - Does not yet group content within quotes (e.g., "hello world" or 'abc def')
 - Multi-character operators (>> and <<) may not be handled at the lexer level
 - Does not support compound operators like &&, ||, or command separators (;)
 - Lexing based only on whitespace separation — needs character-level analysis
--zid m3ak ' ' kifach ikhdmo
 Next Steps:
 - Refactor lexer to work character-by-character for accurate parsing
 - Implement proper quote parsing and escape character handling
 - Expand operator support and refine token categorization

==============================================================================
*/
tokentype analyze_line(char *word)
{
	int i = 0;
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
void lexer(char *line)
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
	print_token(head);
	free_token(head);
}
