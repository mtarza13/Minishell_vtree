#include "lexer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @brief Print a separator line for better output formatting
 */
void	print_separator(void)
{
	printf("================================================\n");
}

/**
 * @brief Print token information in a formatted way
 * @param token Token to display
 * @param index Token index in the list
 */
void	print_token_info(t_token *token, int index)
{
	printf("Token %d:\n", index);
	printf("  Type: %s\n", token_type_str(token->type));
	printf("  Value: %s\n", token->value ? token->value : "(null)");
	printf("  Position: Line %d, Column %d\n", 
		   token->position.line, token->position.column);
	printf("\n");
}

/**
 * @brief Display all tokens in a token list
 * @param tokens Head of token list
 * @param input Original input string
 */
void	display_tokens(t_token *tokens, const char *input)
{
	t_token	*current;
	int		index;

	printf("Input: \"%s\"\n", input);
	printf("Tokens found:\n");
	current = tokens;
	index = 1;
	while (current)
	{
		print_token_info(current, index);
		current = current->next;
		index++;
	}
}

/**
 * @brief Test lexer with a specific input string
 * @param test_name Name of the test case
 * @param input Input string to tokenize
 */
void	run_lexer_test(const char *test_name, const char *input)
{
	t_lexer	*lexer;
	t_token	*tokens;

	printf("=== %s ===\n", test_name);
	lexer = lexer_init(input);
	if (!lexer)
	{
		printf("ERROR: Failed to initialize lexer\n");
		return ;
	}
	tokens = lexer_tokenize(lexer);
	if (!tokens)
	{
		printf("ERROR: Failed to tokenize input\n");
		lexer_free(lexer);
		return ;
	}
	if (tokens->type == TOKEN_ERROR)
	{
		printf("LEXER ERROR: %s\n", tokens->value);
		printf("Position: Line %d, Column %d\n", 
			   tokens->position.line, tokens->position.column);
	}
	else
	{
		display_tokens(tokens, input);
	}
	token_list_free(tokens);
	lexer_free(lexer);
	print_separator();
}

/**
 * @brief Run comprehensive showcase of lexer features
 */
void	run_showcase(void)
{
	printf("🚀 MINISHELL LEXER SHOWCASE 🚀\n");
	printf("Author: mtarza13\n");
	printf("Date: 2025-07-06 08:13:08\n");
	print_separator();
	
	/* Test 1: Basic word tokenization */
	run_lexer_test("Basic Word Tokenization", 
		"echo hello world");
	
	/* Test 2: Pipe operations */
	run_lexer_test("Pipe Operations", 
		"ls -la | grep test | wc -l");
	
	/* Test 3: Input/Output redirection */
	run_lexer_test("Input/Output Redirection", 
		"cat < input.txt > output.txt");
	
	/* Test 4: Append and heredoc */
	run_lexer_test("Append and Heredoc", 
		"echo hello >> log.txt << EOF");
	
	/* Test 5: Environment variables */
	run_lexer_test("Environment Variables", 
		"echo $HOME $USER $PATH $?");
	
	/* Test 6: Single quotes */
	run_lexer_test("Single Quotes", 
		"echo 'Hello World' 'This is a test'");
	
	/* Test 7: Double quotes */
	run_lexer_test("Double Quotes", 
		"echo \"Hello $USER\" \"Path: $HOME\"");
	
	/* Test 8: Mixed quotes and variables */
	run_lexer_test("Mixed Quotes and Variables", 
		"echo 'single' \"double $HOME\" regular $USER");
	
	/* Test 9: Complex command with multiple operators */
	run_lexer_test("Complex Command", 
		"grep 'pattern' < file.txt | sort | uniq > result.txt");
	
	/* Test 10: Error case - unclosed single quote */
	run_lexer_test("Error: Unclosed Single Quote", 
		"echo 'unclosed quote");
	
	/* Test 11: Error case - unclosed double quote */
	run_lexer_test("Error: Unclosed Double Quote", 
		"echo \"unclosed quote");
	
	/* Test 12: Advanced shell features */
	run_lexer_test("Advanced Shell Features", 
		"export PATH=$PATH:/usr/local/bin");
	
	printf("\n🎉 SHOWCASE COMPLETED! 🎉\n");
}

/**
 * @brief Run interactive demo allowing user input
 */
void	run_interactive(void)
{
	char	input[1024];
	t_lexer	*lexer;
	t_token	*tokens;

	printf("🔧 INTERACTIVE MINISHELL LEXER DEMO 🔧\n");
	printf("Author: mtarza13\n");
	printf("Enter shell commands to see how they are tokenized.\n");
	printf("Type 'exit' to quit.\n");
	print_separator();
	
	while (1)
	{
		printf("minishell> ");
		fflush(stdout);
		
		if (!fgets(input, sizeof(input), stdin))
		{
			printf("\nGoodbye!\n");
			break ;
		}
		
		/* Remove newline character */
		input[strcspn(input, "\n")] = 0;
		
		/* Check for exit command */
		if (strcmp(input, "exit") == 0)
		{
			printf("Goodbye!\n");
			break ;
		}
		
		/* Skip empty input */
		if (strlen(input) == 0)
			continue ;
		
		/* Initialize lexer and tokenize */
		lexer = lexer_init(input);
		if (!lexer)
		{
			printf("ERROR: Failed to initialize lexer\n");
			continue ;
		}
		
		tokens = lexer_tokenize(lexer);
		if (!tokens)
		{
			printf("ERROR: Failed to tokenize input\n");
			lexer_free(lexer);
			continue ;
		}
		
		/* Display results */
		if (tokens->type == TOKEN_ERROR)
		{
			printf("LEXER ERROR: %s\n", tokens->value);
			printf("Position: Line %d, Column %d\n", 
				   tokens->position.line, tokens->position.column);
		}
		else
		{
			display_tokens(tokens, input);
		}
		
		/* Cleanup */
		token_list_free(tokens);
		lexer_free(lexer);
		printf("\n");
	}
}

/**
 * @brief Benchmark lexer performance
 * @param test_name Name of the benchmark test
 * @param input Input string to tokenize
 * @param iterations Number of iterations to run
 */
void	benchmark_lexer(const char *test_name, const char *input, int iterations)
{
	clock_t		start_time;
	clock_t		end_time;
	double		cpu_time_used;
	t_lexer		*lexer;
	t_token		*tokens;
	int			i;

	printf("Benchmarking: %s\n", test_name);
	printf("Input: \"%s\"\n", input);
	printf("Iterations: %d\n", iterations);
	
	start_time = clock();
	
	for (i = 0; i < iterations; i++)
	{
		lexer = lexer_init(input);
		if (!lexer)
			continue ;
		
		tokens = lexer_tokenize(lexer);
		if (tokens)
			token_list_free(tokens);
		
		lexer_free(lexer);
	}
	
	end_time = clock();
	cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
	
	printf("Total time: %.6f seconds\n", cpu_time_used);
	printf("Average time per iteration: %.6f seconds\n", 
		   cpu_time_used / iterations);
	printf("Operations per second: %.2f\n\n", 
		   iterations / cpu_time_used);
}

/**
 * @brief Run performance benchmark
 */
void	run_benchmark(void)
{
	printf("⚡ LEXER PERFORMANCE BENCHMARK ⚡\n");
	printf("Author: mtarza13\n");
	printf("Testing lexer performance with various inputs...\n\n");
	
	benchmark_lexer("Simple Command", 
		"echo hello", 10000);
	
	benchmark_lexer("Complex Pipeline", 
		"grep 'pattern' < file.txt | sort | uniq -c | head -10", 5000);
	
	benchmark_lexer("Multiple Redirections", 
		"command < input.txt > output.txt 2>> error.log", 5000);
	
	benchmark_lexer("Environment Variables", 
		"echo $HOME $USER $PATH $PWD $?", 5000);
	
	benchmark_lexer("Quoted Strings", 
		"echo 'single quote' \"double quote with $VAR\"", 5000);
	
	printf("🏁 BENCHMARK COMPLETED! 🏁\n");
}

/**
 * @brief Display help menu
 */
void	display_help(void)
{
	printf("🔧 MINISHELL LEXER DEMO 🔧\n");
	printf("Author: mtarza13\n");
	printf("Date: 2025-07-06 08:13:08\n\n");
	printf("Usage: ./lexer_demo [option]\n\n");
	printf("Options:\n");
	printf("  showcase     - Run comprehensive showcase demo\n");
	printf("  interactive  - Run interactive demo\n");
	printf("  benchmark    - Run performance benchmark\n");
	printf("  help         - Show this help message\n");
	printf("  (no option)  - Run showcase by default\n\n");
	printf("Examples:\n");
	printf("  ./lexer_demo showcase\n");
	printf("  ./lexer_demo interactive\n");
	printf("  ./lexer_demo benchmark\n");
}

/**
 * @brief Main function with command line argument handling
 * @param argc Argument count
 * @param argv Argument vector
 * @return 0 on success, 1 on error
 */
int	main(int argc, char **argv)
{
	if (argc == 1)
	{
		run_showcase();
	}
	else if (argc == 2)
	{
		if (strcmp(argv[1], "showcase") == 0)
			run_showcase();
		else if (strcmp(argv[1], "interactive") == 0)
			run_interactive();
		else if (strcmp(argv[1], "benchmark") == 0)
			run_benchmark();
		else if (strcmp(argv[1], "help") == 0)
			display_help();
		else
		{
			printf("Unknown option: %s\n", argv[1]);
			display_help();
			return (1);
		}
	}
	else
	{
		printf("Too many arguments\n");
		display_help();
		return (1);
	}
	
	return (0);
}
