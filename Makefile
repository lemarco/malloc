
HOSTTYPE := $(shell uname -m)_$(shell uname -s)


NAME = libft_malloc_$(HOSTTYPE).so
LINK = libft_malloc.so

CC = gcc
CFLAGS = -Wall -Werror -Wextra

SRC_PATH = ./src
SRC_NAME =	malloc.c free.c

SRC = $(addprefix $(SRC_PATH)/, $(SRC_NAME))
OBJ = $(SRC:.c=.o)
INC = -I includes

MAL = inc/malloc.h

all: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -shared -o $(NAME) $(OBJ) $(INC) 
	@ln -sf $(NAME) $(LINK)

%.o: %.c
	@$(CC) -c $(CFLAGS) $(INC) $^ -o $@ -fPIC

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(LINK)

re: fclean all

.PHONY: fclean all clean re 