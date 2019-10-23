CC = h5cc
# CFLAGS = -std=c11 -O3 -march=native
CFLAGS = -std=c11 -O0 -g -fsanitize=address,leak
LDFLAGS = -fsanitize=address,leak
LDLIBS = -lm
 
SRC = read_grids.c main.c
OBJ = $(SRC:.c=.o)
 
.PHONY: clean
 
read_grids: $(OBJ)
 
clean:
	rm -f $(OBJ)
