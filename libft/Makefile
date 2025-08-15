NAME = libft.a
CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = ft_strlen.c ft_strdup.c ft_strcmp.c ft_strncmp.c ft_strchr.c \
       ft_substr.c ft_strjoin.c ft_strlcat.c ft_atoi.c ft_itoa.c \
       ft_isalnum.c ft_putnbr_fd.c ft_split.c ft_isdigit.c ft_memset.c \
	   ft_strcpy.c ft_putstr_fd.c ft_memcpy.c ft_strndup.c ft_strncat.c \
	   ft_strncpy.c ft_isalpha.c


OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re