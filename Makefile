NAME = ft_ping

OBJ_DIR		=	./objs
SRC_DIR	=	./srcs

SRC	=	main.c

OBJ	=	$(SRC:.c=.o)
OBJS	=	$(OBJ:%=$(OBJ_DIR)/%)

RM	=	rm -rf

CC			=	gcc
CFLAGS		=	-Wall -Wextra -Werror

all: $(NAME)

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJS): | $(OBJ_DIR)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

.PHONY: all clean fclean re test
