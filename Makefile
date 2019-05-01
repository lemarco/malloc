ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif


NAME = libft_malloc_$(HOSTTYPE).so
LINK = libft_malloc.so

CC = gcc
CFLAGS = -Wall -Werror -Wextra

SRC_PATH = ./src
SRC_NAME =	malloc.c alloc.c free.c realloc.c show_alloc_mem.c unmap.c
SRC = $(addprefix $(SRC_PATH)/, $(SRC_NAME))
OBJ = $(SRC:.c=.o)
INC = -I includes


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
