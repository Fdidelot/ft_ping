NAME = ft_ping

PATH_OBJS	=	./objects/
PATH_SRC	=	./sources/
PATH_HEAD	= ./headers/

SRCS	=	main.c \
			set_struct_and_package.c \
			get_ip_info.c \
			parser.c \
			send_ping.c \
			time.c \
			print.c \
			handle_signal.c
vpath %.c $(PATH_SRC)

HEADERS = ft_ping.h
vpath %.h $(PATH_HEAD)

OBJS	=	$(addprefix $(PATH_OBJS),  $(SRCS:.c=.o))

RM	=	rm -rf

CC			=	gcc
CFLAGS		+=	-Wall -Wextra -Werror
CFLAGS		+=	-I $(PATH_HEAD)
LDLIBS		=	-lm
all: $(NAME)

clean:
	$(RM) $(PATH_OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

$(PATH_OBJS)%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(CFLAGS)

$(OBJS): Makefile $(HEADERS) | $(PATH_OBJS)

$(PATH_OBJS):
	mkdir $@

.PHONY: all clean fclean re test
