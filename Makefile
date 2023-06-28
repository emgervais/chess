NAME	:= chess
CFLAGS	:= -Wextra -Wall -fsanitize=address -g
LIBMLX	:= ./MLX42

LIBS	:= $(LIBMLX)/build/libmlx42.a -lm -lglfw -pthread -ldl
SRCS	:= chess.c init_game.c hooks.c algo.c ft_itoa.c
OBJS	:= ${SRCS:.c=.o}

all: libmlx $(NAME)

libmlx:
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o $(NAME)

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -f $(NAME)

re: clean all

.PHONY: all, clean, fclean, re, libmlx
