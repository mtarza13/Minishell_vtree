NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror

OBJ_DIR = obj
LIBFT_DIR = libft
INCLUDE_DIR = include

INCLUDES = -I./$(INCLUDE_DIR) -I./$(LIBFT_DIR)
LIBFT = $(LIBFT_DIR)/libft.a
LIBS = -lreadline -I/usr/include/readline -L./libft -lft

VPATH = src src/builtins src/env src/executor src/expand src/heredoc \
		src/lexer src/parser src/signals src/utils

MAIN_SRCS = main.c

BUILTIN_SRCS = cd.c echo.c env.c exit.c export.c export_utils.c pwd.c unset.c

EXECUTOR_SRCS = execute_ast.c execute_builtin.c execute_command.c execute_utils.c redirections.c

LEXER_SRCS = tokenize.c token_utils.c

PARSER_SRCS = prompt.c error_check.c parse_ast.c parse_pipe.c parse_redire.c

UTILS_SRCS = ft_malloc.c ft_printf.c ft_utils.c memory_utils.c string_utils.c

ENV_SRCS = env_init.c env_utils.c

SIGNALS_SRCS = signal_handler.c signal_setup.c

HEREDOC_SRCS = heredoc.c heredoc_utils.c

EXPO_SRCS = exp_expand.c exp_count.c exp_quote.c exp_utils.c exp_extract.c

SRCS = $(MAIN_SRCS) $(BUILTIN_SRCS) $(EXECUTOR_SRCS) $(LEXER_SRCS) \
	   $(PARSER_SRCS) $(UTILS_SRCS) $(ENV_SRCS) $(SIGNALS_SRCS) $(HEREDOC_SRCS) $(EXPO_SRCS)

OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

GREEN = \033[38;5;46m
YELLOW = \033[0;33m
RED = \033[0;31m
CYAN = \033[0;36m
RESET = \033[0m

define BANNER
@echo "$(GREEN)"
@echo "███╗   ███╗██╗███╗  ██╗██╗ █████╗ ██╗  ██╗███████╗██╗     ██╗"
@echo "████╗ ████║██║████╗ ██║██║ ██     ██║  ██║██╔════╝██║     ██║"
@echo "██╔████╔██║██║██╔██╗██║██║ █████  ███████║█████╗  ██║     ██║"
@echo "██║╚██╔╝██║██║██║╚████║██     ██╔═██║  ██╔██║     ██║"
@echo "██║ ╚═╝ ██║██║██║ ╚███║██║╚█████╔╝██║  ██║███████╗███████╗██║"
@echo "╚═╝     ╚═╝╚═╝╚═╝  ╚══╝╚═╝ ╚════╝ ╚═╝  ╚═╝╚══════╝╚══════╝╚═╝"
@echo "                       by mtarza & yabarhda                    "
@echo "$(RESET)"
endef

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@$(BANNER)
	@echo "$(YELLOW)Linking executable...$(RESET)"
	@$(CC) $(OBJS) $(LIBS) -o $(NAME)
	@echo "$(GREEN)Success! $(NAME) is ready. ✔$(RESET)"

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "$(CYAN)Compiling:$(RESET) $<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@make -s -C $(LIBFT_DIR)

clean:
	@rm -rf $(OBJ_DIR)
	@make -s -C $(LIBFT_DIR) clean
	@echo "$(RED)Object files wiped.$(RESET)"

fclean: clean
	@rm -f $(NAME)
	@make -s -C $(LIBFT_DIR) fclean
	@echo "$(RED)Full cleanse complete. Binary and objects removed.$(RESET)"

re:
	@$(MAKE) -s fclean
	@$(MAKE) -s all

.PHONY: all clean fclean re