#ifndef READ_GRIDS_H
#define READ_GRIDS_H

struct grid {
    int n_cell[3];
    double* values;
    double box_size;
    int snapshot;
};

struct grid read_grid(const char* grid_dir, const int snapshot);
void free_grid(struct grid* grid);

#endif
