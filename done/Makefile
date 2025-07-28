CC = cc 
CFLAG = -Wall -Wextra -Werror -g


SRCS = src/main.c \
	   lexer/tokenize.c \
	   parser/parse_command.c \

INCLUDES =  -I./include -L./libft 
LIBS = -lft -lreadline
NAME= minishell
LIBFT = libft/libft.a


all: $(NAME)
$(NAME): $(SRCS)
	$(CC)  $(FLAG) $(INCLUDES)  -o $(NAME) $(SRCS) $(LIBS)
clean:
	@make -c $(NAME)
fclean:
	clean
re: fclean all
.PHONY :all clean fclean re
