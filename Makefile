CC = h5cc
CFLAGS = -std=c11 -O3 -march=native
LDFLAGS = 
LDLIBS = -lm
 
SRC = read_grids.c main.c
OBJ = $(SRC:.c=.o)
 
.PHONY: clean
 
read_grids: $(OBJ)
 
clean:
	rm -f $(OBJ)
