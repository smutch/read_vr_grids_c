#include <stdio.h>

#include "read_grids.h"

int main(int argc, char* argv[])
{
    struct grid grid = read_grid("/fred/oz009/pelahi/GENESIS/L105_N2048/den/", 30);

    printf("Box size = %g\n", grid.box_size);
    printf("n_cell = [%d,%d,%d]\n", grid.n_cell[0], grid.n_cell[1],
        grid.n_cell[2]);
    printf("Grid[0, 0, 0] = %g\n\n", grid.values[0]);
    printf("\n------\nDEBUG - every 1e7th element:\n");

    const int total_n_cell = grid.n_cell[0] * grid.n_cell[1] * grid.n_cell[2];
    for (int ii = 0; ii < total_n_cell; ii += 1e7) {
        printf("%g ", grid.values[ii]);
    }
    printf("\n------\n");

    // Brad: The grid can be accessed as `grid.values`.
    //       Just be sure to call `free_grid` as below to free the allocation.`

    free_grid(&grid);

    return 0;
}
